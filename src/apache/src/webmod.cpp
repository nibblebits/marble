#include "webmod.h"
#include "apache_mod.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "misc.h"
#include <string>
#include <ctype.h>
#include <iostream>
#include <sstream>

MultipartFileObject::MultipartFileObject(Class* c) : Object(c)
{

}

MultipartFileObject::~MultipartFileObject()
{

}

std::shared_ptr<Object> MultipartFileObject::newInstance(Class* c)
{
    return std::make_shared<MultipartFileObject>(c);
}

WebModuleRequestArgumentsObject::WebModuleRequestArgumentsObject(Class* c) : Object(c)
{

}

WebModuleRequestArgumentsObject::~WebModuleRequestArgumentsObject()
{

}

WebModulePOSTContentObject::WebModulePOSTContentObject(Class* c) : Object(c)
{
    
}

WebModulePOSTContentObject::~WebModulePOSTContentObject()
{

}


WebModulePOSTFileContentObject::WebModulePOSTFileContentObject(Class* c) : Object(c)
{
    
}

WebModulePOSTFileContentObject::~WebModulePOSTFileContentObject()
{

}


WebModuleObject::WebModuleObject(Class* c) : Object(c)
{
    this->req = NULL;
}

WebModuleObject::~WebModuleObject()
{

}



WebModule::WebModule() : Module("webmod", "Web Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

WebModule::~WebModule()
{

}

void WebModule::Init()
{
    log("Web Module initialised", LOG_LEVEL_NOTICE);

    /* RequestArguments Class*/
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("RequestArguments");
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleRequestArgumentsObject> args_obj = std::dynamic_pointer_cast<WebModuleRequestArgumentsObject>(object);
        return_value->type = VALUE_TYPE_STRING;

        if (args_obj->arguments.find(arguments[0].svalue) == args_obj->arguments.end())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})));
        }

        return_value->svalue = args_obj->arguments[arguments[0].svalue];
    });

    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("boolean"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleRequestArgumentsObject> args_obj = std::dynamic_pointer_cast<WebModuleRequestArgumentsObject>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = (args_obj->arguments.find(arguments[0].svalue) != args_obj->arguments.end());
    });
    /* End of RequestArguments Class */

    /* MultipartFile class*/
    c = this->getModuleSystem()->getClassSystem()->registerClass("MultipartFile");
    c->setDescriptorObject(std::make_shared<MultipartFileObject>(c));
    c->registerFunction("getType", {}, VarType::fromString("string"), WebModule::MultipartFile_getType);
    c->registerFunction("getName", {}, VarType::fromString("string"), WebModule::MultipartFile_getName);
    c->registerFunction("getPath", {}, VarType::fromString("string"), WebModule::MultipartFile_getPath);
    /* End of MultipartFile class */

    /* FileContent class */
    c = this->getModuleSystem()->getClassSystem()->registerClass("FileContent");
    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("boolean"), WebModule::FileContent_has);
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("MultipartFile"), WebModule::FileContent_get);
    
    /* End of FileContent class */

    /* PostContent class */
    c = this->getModuleSystem()->getClassSystem()->registerClass("PostContent");
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModulePOSTContentObject> post_obj = std::dynamic_pointer_cast<WebModulePOSTContentObject>(object);
        return_value->type = VALUE_TYPE_STRING;

        if (post_obj->content.find(arguments[0].svalue) == post_obj->content.end())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})));
        }

        return_value->svalue = post_obj->content[arguments[0].svalue];
    });

    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModulePOSTContentObject> post_obj = std::dynamic_pointer_cast<WebModulePOSTContentObject>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = (post_obj->content.find(arguments[0].svalue) != post_obj->content.end());
    });
    /* End of POSTContent class */

    /* Request class */
    c = this->getModuleSystem()->getClassSystem()->registerClass("Request");
    c->registerFunction("getUri", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->request_uri;
    });

    c->registerFunction("getMethod", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->request_method;
    });

    c->registerFunction("getArguments", {}, VarType::fromString("RequestArguments"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = web_mod_obj->request_arguments;
    });

    c->registerFunction("getContent", {}, VarType::fromString("PostContent"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = web_mod_obj->content;
    });

    c->registerFunction("getFileContent", {}, VarType::fromString("FileContent"), WebModule::Request_getFileContent);

    c->registerFunction("getRequesterIP", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->requester_ip;
    });


    // Used to set response headers for the given HTTP request
    c->registerFunction("setResponseHeader", {VarType::fromString("string"), VarType::fromString("string")}, {VarType::fromString("void")}, WebModule::Request_setResponseHeader);

    /* End of Request class */

}

void WebModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to Request. 
    Scope* root_scope = interpreter->getRootScope();
    std::shared_ptr<WebModuleObject> object = std::make_shared<WebModuleObject>(this->getModuleSystem()->getClassSystem()->getClassByName("Request"));
    object->request_arguments = std::make_shared<WebModuleRequestArgumentsObject>(this->getModuleSystem()->getClassSystem()->getClassByName("RequestArguments"));
    object->content = std::make_shared<WebModulePOSTContentObject>(this->getModuleSystem()->getClassSystem()->getClassByName("PostContent"));
    object->file_content = std::make_shared<WebModulePOSTFileContentObject>(this->getModuleSystem()->getClassSystem()->getClassByName("FileContent"));
    root_scope->createVariable("Request", "Request", object);
}

void WebModule::parseRequest(Interpreter* interpreter, request_rec* req)
{
    Scope* root_scope = interpreter->getRootScope();
    Variable* variable = root_scope->getVariableAnyScope("Request");

    std::shared_ptr<WebModuleObject> object = std::dynamic_pointer_cast<WebModuleObject>(variable->value.ovalue);
    object->req = (struct request_rec*) req;
    object->request_uri = req->unparsed_uri;
    object->requester_ip = req->useragent_ip;
    object->request_method = req->method;
    object->request_arguments->arguments = parseGet(req);

    const char* contentType_cstr = apr_table_get(req->headers_in, "content-type");
    if (!contentType_cstr)
        return;

    std::string contentType = std::string(contentType_cstr);

    if (contentType == "application/x-www-form-urlencoded")
    {
        object->content->content = parsePost(req);
    }
    else if(contentType.find("multipart/form-data") == 0)
    {
       // Let's attempt to parse multipart form data
       struct multipart_parse mparse = parseMultipartFormData(req, interpreter);
       // Put the non file field into the WebModuleObject's POST content
       object->content->content = mparse.post_field_content;
       object->file_content->content = mparse.file_field_content;
    }
}

std::string WebModule::readNextInBuffer(char* buf_end, char** ptr, std::string delm)
{
    std::string result = "";
    char* current = *ptr;
    if (current > buf_end)
        return "";

    char *end =  (char*) memmem((void*) current, (buf_end-current), (void*) delm.c_str(), delm.size());
    if (end == NULL)
        return "";
    
    int diff = (end-current);
    
    for (int i = 0; i < diff; i++)
    {
        result += current[i];
    }
    // We want to go past the delemiter
    char* new_ptr = end + delm.size();
    if (new_ptr <= buf_end)
        *ptr = new_ptr;
    else
        *ptr = buf_end;

    return result;
}

std::string WebModule::extractValueFromString(std::string str)
{
    std::string processed_string = "";
    bool began_string = false;
    for (int i = 0; i < str.size(); i++)
    {
        if (str.at(i) == '"')
        {
            // If we have already began this string then we are finally done
            if (began_string)
                break;

            // We have began the string
            began_string = true;
        }
        else if(str.at(i) == '\\')
        {
            if (str.size() >= i+1 && str.at(i+1) == '"')
            {
                // We have an escape sequence character let's ignore this byte and skip over the next one while adding it to this string
                processed_string += str.at(i+1);
                i++;
            }
            else
            {
                // We are not escaping anything here let's make sure we add this character to the string
                processed_string += str.at(i);
            }
        }
        else
        {
            processed_string += str.at(i);
        }
    }

    return processed_string;
}

std::map<std::string, std::string> WebModule::parseKeyAndValueForString(std::string str)
{
    std::map<std::string, std::string> m;

    std::vector<std::string> outer_splits = str_split(str, ";");
    for (int i = 0; i < outer_splits.size(); i++)
    {
        std::string value = "";
        std::string os = outer_splits[i];
        std::vector<std::string> inner_splits = str_split(os, "=");
        std::string key = inner_splits.at(0).substr(inner_splits.at(0).find_first_not_of(" "));
        // Let's convert the key to lowercase to be used for case insensitivity
        for (int c = 0; c < key.size(); c++)
            key[c] = std::tolower(key[c]);
        
        if (inner_splits.size() > 1)
        {
            value = extractValueFromString(inner_splits[1]);
        }
        m[key] = value;
    }
    
    return m;
}

std::map<std::string, std::map<std::string, std::string>> WebModule::handleMultipartHeaderInfo(std::string header_info)
{
    std::map<std::string, std::map<std::string, std::string>> parse_map;
    // Let's first split the header info into it's individual headers
    std::vector<std::string> headers = str_split(header_info, "\r\n");
    for(std::string header : headers)
    {
        std::map<std::string, std::string> value_map;
        std::vector<std::string> header_split = str_split(header, ":");
        std::string header_field_name = header_split.at(0);
        // the header field name should be converted to lowercase for case insensitivity
        for (int c = 0; c < header_field_name.size(); c++)
            header_field_name[c] = std::tolower(header_field_name[c]);

        if (header_split.size() > 1)
        {
            std::string header_field_value = header_split.at(1);
            // We want to ignore trailing white spaces for the value
            std::size_t spos = header_field_value.find_first_not_of(" ");
            header_field_value = header_field_value.substr(spos, header_field_value.size());
            value_map = parseKeyAndValueForString(header_field_value);
            parse_map[header_field_name] = value_map;
        }
    }

    return parse_map;
}
struct multipart_parse WebModule::parseMultipartFormData(request_rec* req, Interpreter* interpreter)
{
    struct multipart_parse p;
    const char* contentType_cstr = apr_table_get(req->headers_in, "content-type");
    if (!contentType_cstr)
        return p;

    std::string contentType = std::string(contentType_cstr);
    if(contentType.find("multipart/form-data") != 0)
    {
        // We only parse multipart
        return p;
    }
    
    apr_off_t size;
    char* buf = NULL;
    char* pos = NULL;
    char* end = NULL;
    if(util_read(req, (const char**) &buf, &size) != OK) {
        return p;
    }
    pos = buf;
    end = buf+size;
    std::string signature = readNextInBuffer(end, &pos, "\r\n");
    while(pos < end)
    {
        bool is_file = false;
        std::string header_info = readNextInBuffer(end, &pos, "\r\n\r\n");

        std::map<std::string, std::map<std::string, std::string>> header_info_map = handleMultipartHeaderInfo(header_info);
        if (header_info_map.find("content-disposition") == header_info_map.end())
        {
            // Content-Disposition is a required header for multipart data
            return p;
        }

        std::map<std::string, std::string> content_disposition_properties = header_info_map.at("content-disposition");
        if (content_disposition_properties.find("name") == content_disposition_properties.end())
        {
            // We need a name so let's just return here as none was provided
            return p;
        }

        if (content_disposition_properties.find("filename") != content_disposition_properties.end())
        {
            is_file = true;
            if (header_info_map.find("content-type") == header_info_map.end())
            {
                // The client has specified that this is a file but has not provided a content type. Lets reject it
                return p;
            }
        }

    
        std::string data = readNextInBuffer(end, &pos, signature);

        if (!is_file)
        {
            // This is not a file so lets add this to our post field content
            p.post_field_content[content_disposition_properties["name"]] = data;
        }
        else
        {
            std::shared_ptr<MultipartFileObject> file_obj = std::dynamic_pointer_cast<MultipartFileObject>(Object::create(interpreter->getClassSystem()->getClassByName("MultipartFile")));
            file_obj->name = content_disposition_properties["filename"];
            file_obj->type = header_info_map["content-type"].begin()->first;

            // Let's write this file to temp
            file_obj->path = writeTemp(data.c_str(), data.size());
            p.file_field_content[content_disposition_properties["name"]] = file_obj;
        }
                
        if (pos < end)
        {
            if (*pos == '-')
            {
                // We have reached the end --
                break;
            }
        }
    }

    return p;
}

std::map<std::string, std::string> WebModule::parsePost(request_rec* req)
{
    std::map<std::string, std::string> m;
    const char* contentType_cstr = apr_table_get(req->headers_in, "content-type");
    if (!contentType_cstr)
        return m;
    
    std::string contentType = std::string(contentType_cstr);
    if (contentType == "application/x-www-form-urlencoded")
    {
        keyValuePair* kvp = readPost(req);
        if (kvp == NULL)
            return m;

        for (int i = 0; &kvp[i]; i++) {
            if (kvp[i].key) {
                m[kvp[i].key] = (kvp[i].value ? kvp[i].value : "");
            }
            else
                break;
        }

        return m;
    }

}

std::map<std::string, std::string> WebModule::parseGet(request_rec* req)
{
    std::map<std::string, std::string> m;

    if (req->parsed_uri.query == NULL)
        return m;
    m["query"] = req->parsed_uri.query;

    std::string query_str = req->parsed_uri.query;
    std::string name = "";
    std::string value = "";
    bool do_name = true;
    for (int i = 0; i < query_str.size(); i++)
    {
        char c = query_str[i];
        if (c == '=')
        {
            do_name = false;
        }
        else if(c == '&')
        {
            m[urlDecode(name)] = urlDecode(value);
            name = "";
            value = "";
            do_name = true;
        }
        else
        {
            if (do_name)
                name += c;
            else
                value += c;
        }
    }

    if (name != "")
        m[urlDecode(name)] = urlDecode(value);
    
    return m;
}

void WebModule::Request_setResponseHeader(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleObject> wm_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
    request_rec* req = wm_obj->req;
    apr_table_set(req->headers_out, values[0].svalue.c_str(), values[1].svalue.c_str());
}

void WebModule::Request_getFileContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleObject> wm_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
    return_value->set(wm_obj->file_content);
}

void WebModule::FileContent_has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
     std::shared_ptr<WebModulePOSTFileContentObject> file_content_obj = std::dynamic_pointer_cast<WebModulePOSTFileContentObject>(object);
     return_value->set(file_content_obj->content.find(values[0].svalue) != file_content_obj->content.end());
}

void WebModule::FileContent_get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModulePOSTFileContentObject> file_content_obj = std::dynamic_pointer_cast<WebModulePOSTFileContentObject>(object);
    if(file_content_obj->content.find(values[0].svalue) == file_content_obj->content.end())
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})));
    }

    return_value->set(file_content_obj->content[values[0].svalue]);
}

void WebModule::MultipartFile_getType(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->type);
}

void WebModule::MultipartFile_getName(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->name); 
}

void WebModule::MultipartFile_getPath(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MultipartFileObject> m_obj = std::dynamic_pointer_cast<MultipartFileObject>(object);
    return_value->set(m_obj->path); 
}