#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>

#include "apache_mod.h"
#include "interpreter.h"
#include "basesystemhandler.h"
#include "webmod.h"
#include "misc.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "exceptions/timeoutexception.h"

// request handler example

// request_rec Struct Reference
// http://ci.apache.org/projects/httpd/trunk/doxygen/structrequest__rec.html


typedef struct
{
    char context[256];
    char* max_memory;
    char* config_location;
    // The permissions that all interpreters are bound to. The configuration file loads these.
    std::shared_ptr<PermissionsObject> set_permissions;
    // The timeout that scripts should stop when they reach
    int timeout = 0;
} configuration;

static configuration config;
BaseSystemHandler* baseHandler = NULL;
ModuleSystem* moduleSystem = NULL;
WebModule* webModule = NULL;

static int marble_handler(request_rec *req);
static void marble_register_hooks(apr_pool_t* p);
const char* set_max_memory(cmd_parms *cmd, void *cfg_void, const char *arg);
const char* set_config_location(cmd_parms* cmd, void* cfg_void, const char* arg);
void *create_dir_conf(apr_pool_t *pool, char *context);
void *merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD);
bool loadConfiguration(std::string configFileName, configuration* conf);
int util_read(request_rec *r, const char **rbuf, apr_off_t *size);

bool first_run = true;

static const command_rec directives[] =
{
    // maxMemory does not yet work
    AP_INIT_TAKE1("maxMemory", (const char* (*)())set_max_memory, NULL, ACCESS_CONF, "Sets the max memory that marble can use"),
    AP_INIT_TAKE1("configLocation", (const char* (*)())set_config_location, NULL, ACCESS_CONF, "Sets the location of the configuration file that should be used"),
    { NULL }
};

extern "C"
{
    /* Dispatch list for API hooks */
    module AP_MODULE_DECLARE_DATA mod_marble_module = {
        STANDARD20_MODULE_STUFF, 
        create_dir_conf, /* create per-dir    config structures */
        merge_dir_conf, /* merge  per-dir    config structures */
        NULL, /* create per-server config structures */
        NULL, /* merge  per-server config structures */
        directives, /* table of config file commands       */
        marble_register_hooks  /* register hooks */
    };
}


std::vector<std::string> split(std::string str, std::string delim)
{
    std::vector<std::string> splits;
    std::size_t pos = 0;
    std::size_t last_pos = pos;
    while((pos = str.find(delim, last_pos)) != std::string::npos)
    {
        std::string s = str.substr(last_pos, (pos-last_pos));
        splits.push_back(s);
        last_pos = pos + delim.size();
    }
    
    // Now lets get the final part
    if (str.begin()+last_pos != str.end())
        splits.push_back(str.substr(last_pos, str.size()-1));
        
    return splits;
}



int util_read(request_rec *r, const char **rbuf, apr_off_t *size)
{
    /*~~~~~~~~*/
    int rc = OK;
    /*~~~~~~~~*/

    if((rc = ap_setup_client_block(r, REQUEST_CHUNKED_ERROR))) {
        return(rc);
    }

    if(ap_should_client_block(r)) {

        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
        char         argsbuffer[HUGE_STRING_LEN];
        apr_off_t    rsize, len_read, rpos = 0;
        apr_off_t length = r->remaining;
        /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

        *rbuf = (const char *) apr_pcalloc(r->pool, (apr_size_t) (length + 1));
        *size = length;
        while((len_read = ap_get_client_block(r, argsbuffer, sizeof(argsbuffer))) > 0) {
            if((rpos + len_read) > length) {
                rsize = length - rpos;
            }
            else {
                rsize = len_read;
            }

            memcpy((char *) *rbuf + rpos, argsbuffer, (size_t) rsize);
            rpos += rsize;
        }
    }
    return(rc);
}


keyValuePair *readPost(request_rec *r) {
    apr_array_header_t *pairs = NULL;
    apr_off_t len;
    apr_size_t size;
    int res;
    int i = 0;
    char *buffer;
    keyValuePair *kvp;

    res = ap_parse_form_data(r, NULL, &pairs, -1, HUGE_STRING_LEN);
    if (res != OK || !pairs) return NULL; /* Return NULL if we failed or if there are is no POST data */
    kvp = (keyValuePair*) apr_pcalloc((apr_pool_t*) r->pool, sizeof(keyValuePair) * (pairs->nelts + 1));
    while (pairs && !apr_is_empty_array(pairs)) {
        ap_form_pair_t *pair = (ap_form_pair_t *) apr_array_pop(pairs);
        apr_brigade_length(pair->value, 1, &len);
        size = (apr_size_t) len;
        buffer = (char*)apr_palloc(r->pool, size + 1);
        apr_brigade_flatten(pair->value, buffer, &size);
        buffer[len] = 0;
        kvp[i].key = apr_pstrdup(r->pool, pair->name);
        kvp[i].value = buffer;
        i++;
    }
    return kvp;
}

static bool valid_filename(std::string filename)
{
    // We need to ensure this filename links to a marble file ".marble" that can be processed
    std::vector<std::string> vec = split(filename, "/");
    if (vec.empty())
        return false;
    
    std::string final_part = vec[vec.size()-1];
    vec = split(final_part, ".");
    if (vec.empty())
        return false;
    std::string file_ext = vec[vec.size()-1];
    if (file_ext != "marble")
        return false;   
    return true; 
}

std::string format_log_entry(LogEntry entry)
{
    std::string type = "";
    switch(entry.level)
    {
        case LOG_LEVEL_NOTICE:
            type = "Notice";
        break;
        case LOG_LEVEL_WARNING:
            type = "Warning";
        break;
        case LOG_LEVEL_ERROR:
            type = "Error";
        break;
        
        default:
            type = "Unknown Log Entry";
    }
    std::string message = "<b>" + type + "</b>" + ": <i>" + entry.message + "</i> ";
    if (entry.posInfo.line != -1) 
       message += "on line " + std::to_string(entry.posInfo.line) + ", column "  + std::to_string(entry.posInfo.col);
       message += " in file " + entry.posInfo.filename + "<br />";
    return message;
}

std::string getDirectoryForFilename(std::string filename)
{
    std::vector<std::string> filename_split_vec = split(filename, "/");
    std::string path = "";
    for (int i = 0; i < filename_split_vec.size()-1; i++)
    {
        std::string s = filename_split_vec.at(i);
        path += s + "/";
    }

    return path;
}

static int marble_handler(request_rec *req)
{
    
    int rc, exists;
    apr_finfo_t finfo;

    if (!req->handler) 
        return DECLINED;
       

    std::string filename = req->filename;
    if (req->handler && strcmp(req->handler, "marble/") && !isFile(filename))
    {
        // This appears to be a directory so lets append index.marble to the end
        filename += "index.marble";
    }

    if(!valid_filename(filename))
        return DECLINED;

    req->content_type = "";
    
    // ap_table_set is too old API
    apr_table_set(req->headers_out, "X-Content-Type-Options", "nosniff");
    apr_table_set(req->headers_out, "Content-Type", "text/html");

    // Let's check that the file actually exists.
    rc = apr_stat(&finfo, filename.c_str(), APR_FINFO_MIN, req->pool);
    if (rc == APR_SUCCESS)
    {
        exists = ((finfo.filetype != APR_NOFILE) 
                    && !(finfo.filetype & APR_DIR));
        if (!exists)
            return DECLINED;
    }
    else 
        return DECLINED;

    int statusCode = 200;
    if (!req->header_only) {
        // Load marble configuration
        configuration* conf = (configuration*) ap_get_module_config(req->per_dir_config, &mod_marble_module);
        loadConfiguration(conf->config_location, conf);
        Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
        interpreter.setModuleSystem(moduleSystem);
        interpreter.setOutputFunction([&](const char* data, int length) {
            ap_rwrite(data, length, req);
        });


        // Inject the permissions loaded from the configuration into our root scope granting access
        interpreter.getGlobalScope()->permissions = conf->set_permissions;

        // Let's set the timeout for this interpreter
        interpreter.setTimeout(conf->timeout);
        std::map<std::string, std::string> cookies = webModule->parseCookies(req);
        // We must get the session cookie if it exists if it does not we must set it
        std::string session_cookie_value = "";
        if (cookies.find("marble_sess") == cookies.end())
        {
            // Session key does not exist lets create one
            session_cookie_value = random_hex(50);
            std::string session_cookie_header = "marble_sess=" + session_cookie_value;
            apr_table_set(req->headers_out, "Set-Cookie", session_cookie_header.c_str());
        }
        else
        {
            session_cookie_value = cookies["marble_sess"];
        }

        // Set the session key for this interpreter
        interpreter.properties["session_key"] = session_cookie_value;

        Logger* logger = interpreter.getLogger();

        // We must change our working directory so let's get the directory we are accessing
        std::string filename_str = filename;
        std::string working_directory = getDirectoryForFilename(filename_str);
        if(chdir(working_directory.c_str()) != 0)
            logger->warn("Failed to change the working directory. You may need to provide absolute paths");

        // Let's let the WebModule know about our request
        webModule->parseRequest(&interpreter, req);

        try
        {
            try
            {
                interpreter.runScript(filename.c_str());
            }
            catch(TimeoutException& ex)
            {
                logger->error("The script has timed out; If you need more time please increase the timeout limit");
            }
        }
        catch(...)
        {
            // If the logger has errors then this exception was thrown because of them. This wouldnt be an internal server error we will output these errors later.
            if (!logger->hasErrors())
                return HTTP_INTERNAL_SERVER_ERROR;
        }
        
        if (logger->hasLog())
        {
            // The logger has a log so lets output it
            for(LogEntry entry : logger->entries)
            {
                ap_rputs(format_log_entry(entry).c_str(), req);
            }
        }

        statusCode = std::stoi(interpreter.properties["response_code"]);
    }
    
    
    // OK = 0 but when returning 200 we get an error so we need to do this
    return statusCode == 200 ? OK : statusCode;
}

bool loadConfiguration(std::string configFileName, configuration* conf=NULL)
{
   std::cout << "LOADING MARBLE CONFIGURATION: " << configFileName << std::endl;
    Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
    interpreter.setOutputFunction([](const char* data, int length) {
        for (int i = 0; i < length; i++)
            std::cout << data[i];
    });
    // Configurations should not be bound to permissions.
    interpreter.setNoPermissionRestrictions(true);
    interpreter.setModuleSystem(moduleSystem);
    try
    {
        interpreter.runScript(configFileName.c_str());
    }
    catch(...)
    {
        std::cout << "Failed to load or run the configuration script: " << configFileName << std::endl;
        throw;
    }

    /* The set permissions become equal to the permissions of the configuration interpreter
       Do be aware this type of setup only works in a single threaded environment in a multithreaded environment the permission object must be cloned
       when inserting it into interpreters that must be run later. */ 
    if (conf != NULL)
    {
        conf->set_permissions = interpreter.getGlobalScope()->permissions;

        // We also want to remember the timeout this configuration setup for us
        conf->timeout = interpreter.getTimeout();
    }
    return true;
}

/**
 * Initialises this module's child request
 */
static void child_init(apr_pool_t*, server_rec* s)
{
  
}

/**
 * Load everything before our process is forked
 */
int post_config(apr_pool_t *pconf, apr_pool_t *plog,
                          apr_pool_t *ptemp, server_rec *s)
{
    baseHandler = new BaseSystemHandler();
    moduleSystem = new ModuleSystem(baseHandler->getClassSystem(), baseHandler->getFunctionSystem());

    webModule = new WebModule();
    moduleSystem->addModule(webModule);

    // We must load the base configuration file in charge of loading modules
    loadConfiguration("/etc/marble/loaderconfig.marble");
    return OK;
}

static void marble_register_hooks(apr_pool_t* p)
{
    printf("\n ** marble_register_hooks  **\n\n");
    ap_hook_handler(marble_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(child_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_post_config(post_config, NULL, NULL, APR_HOOK_MIDDLE);
}


const char* set_max_memory(cmd_parms *cmd, void *cfg_void, const char *arg)
{
    printf("\n ** set_max_memory **\n\n");
    configuration* cfg = (configuration*) cfg_void;
    cfg->max_memory = (char*) arg;
    return NULL;
}

const char* set_config_location(cmd_parms *cmd, void *cfg_void, const char *arg)
{
    printf("\n ** set_config_location **\n\n");
    configuration* cfg = (configuration*) cfg_void;
    cfg->config_location = (char*) arg;
    return NULL;
}

void *create_dir_conf(apr_pool_t *pool, char *context)
{
    context = context ? context : (char*) "Newly created configuration";

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    configuration* cfg = (configuration*) apr_pcalloc(pool, sizeof(configuration));
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    if(cfg)
    {
         /* Set some default values */
         strcpy(cfg->context, context);
         cfg->max_memory = (char*) "10MB";
         cfg->config_location = (char*) "/noconfig";
    }

    return cfg;
}

void *merge_dir_conf(apr_pool_t *pool, void *BASE, void *ADD)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    configuration* base = (configuration*) BASE;
    configuration* add = (configuration*) ADD;
    configuration* conf = (configuration*) create_dir_conf(pool, (char*) "Merged configuration");
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    conf->max_memory = add->max_memory;
    conf->config_location = add->config_location;
    conf->set_permissions = add->set_permissions;
    return conf;
}


