/* 
 *  mod_helloworld.c -- Apache sample helloworld module
 *  [Autogenerated via ``apxs -n helloworld -g'']
 *
 *  To play with this sample module first compile it into a
 *  DSO file and install it into Apache's modules directory 
 *  by running:
 *
 *    $ apxs -c -i mod_helloworld.c
 *
 *  Then activate it in Apache's httpd.conf file for instance
 *  for the URL /helloworld in as follows:
 *
 *    # httpd.conf
 *    LoadModule helloworld_module modules/mod_helloworld.so
 *    <Location /hello>
 *      SetHandler helloworld
 *    </Location>
 *
 *  Then after restarting Apache via
 *
 *    $ apachectl restart
 *
 *  you immediately can request the URL /helloworld and watch for the
 *  output of this module. This can be achieved for instance via:
 *
 *    $ lynx -mime_header http://localhost/hello
 */ 

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"
#include "interpreter.h"
#include <string>
#include <vector>

// request handler example

// request_rec Struct Reference
// http://ci.apache.org/projects/httpd/trunk/doxygen/structrequest__rec.html

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

static int marble_handler(request_rec *req)
{
    int rc, exists;
    apr_finfo_t finfo;
    
    if (!req->handler || strcmp(req->handler, "marble")) 
        return DECLINED;
       
    if(!valid_filename(std::string(req->filename)))
        return DECLINED;
          
    req->content_type = "text/html;charset=UTF-8";

    // ap_table_set is too old API
    apr_table_set(req->headers_out, "X-Content-Type-Options", "nosniff");
    
    // Let's check that the file actually exists.
    rc = apr_stat(&finfo, req->filename, APR_FINFO_MIN, req->pool);
    if (rc == APR_SUCCESS)
    {
        exists = ((finfo.filetype != APR_NOFILE) 
                    && !(finfo.filetype & APR_DIR));
        if (!exists)
            return HTTP_NOT_FOUND;
    }
    else 
        return HTTP_NOT_FOUND;
    
    if (!req->header_only) {
        Interpreter interpreter;
        interpreter.setOutputFunction([&](const char* data) {
            ap_rputs(data, req);
        });
        Logger* logger = interpreter.getLogger();
        try
        {
            interpreter.runScript(req->filename);
        } catch(std::exception &ex)
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
                std::string message = entry.message;
                message = "<b>" + message + "</b>" + "<br />";
                ap_rputs(message.c_str(), req);
            }
        }
    }

    return OK;
}

static void marble_register_hooks(apr_pool_t *p)
{
    printf("\n ** helloworld_register_hooks  **\n\n");
    ap_hook_handler(marble_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
 module AP_MODULE_DECLARE_DATA mod_marble_module = {
    STANDARD20_MODULE_STUFF, 
    NULL, /* create per-dir    config structures */
    NULL, /* merge  per-dir    config structures */
    NULL, /* create per-server config structures */
    NULL, /* merge  per-server config structures */
    NULL, /* table of config file commands       */
    marble_register_hooks  /* register hooks */
};

/* Sadly apxs does not compile with G++. I have had to compile with g++ instead of apxs and therefore I have had to duplicate the AP_MODULE_DECLARE_DATA structure as apache requires two symbols. "mod_modulename" and "modulename". Which is usually handled automatically by apxs upon compiling.
*/
module AP_MODULE_DECLARE_DATA marble_module = {
    STANDARD20_MODULE_STUFF, 
    NULL, /* create per-dir    config structures */
    NULL, /* merge  per-dir    config structures */
    NULL, /* create per-server config structures */
    NULL, /* merge  per-server config structures */
    NULL, /* table of config file commands       */
    marble_register_hooks  /* register hooks */
};
