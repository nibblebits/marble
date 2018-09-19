/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "webmod.h"
#include "apache_mod.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "responseobject.h"
#include "requestargumentsobject.h"
#include "multipartfileobject.h"
#include "modulesystem.h"
#include "cookiepermission.h"
#include "headerpermission.h"

#include "misc.h"
#include "array.h"
#include "object.h"
#include <string>
#include <ctype.h>
#include <iostream>
#include <sstream>


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



WebModule::WebModule() : Module("webmod", "Web Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "ADD_HELP_PATH_HERE")
{

}

WebModule::~WebModule()
{

}

void WebModule::Init()
{
    log("Web Module initialised", LOG_LEVEL_NOTICE);

    // Register the CookiePermission class
    CookiePermission::registerClass(this->getModuleSystem());

    // Register the HeaderPermission class
    HeaderPermission::registerClass(this->getModuleSystem());

    // Register the Response class
    WebModule_ResponseObject::registerClass(this->getModuleSystem());

    // Register the RequestArguments class
    WebModuleRequestArgumentsObject::registerClass(this->getModuleSystem());

    // Register the MultipartFile class
    MultipartFileObject::registerClass(this->getModuleSystem());

    /* FileContent class */
    /**
     * class FileContent
     * 
     * Holds MultipartFile's that were uploaded to your apache2 web server
     */
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("FileContent");
    /**
     * @class FileContent
     * Returns true if we have a MultipartFile with the index name provided
     * function has(string index_name) : boolean
     */
    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("boolean"), WebModule::FileContent_has);
    /**
     * @class FileContent
     * Returns the MultipartFile with the given index name
     * function get(string index_name) : MultipartFile
     */
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("MultipartFile"), WebModule::FileContent_get);
     /**
     * @class FileContent
     * Returns true if we have a MultipartFile array with the given index name
     * function hasArray(string index_name) : boolean
     */
    c->registerFunction("hasArray", {VarType::fromString("string")}, VarType::fromString("boolean"), WebModule::FileContent_hasArray);
      /**
     * @class FileContent
     * Returns the MultipartFile array with the given index name
     * function getArray(string index_name) : MultipartFile[]
     */
    c->registerFunction("getArray", {VarType::fromString("string")}, VarType::fromString("MultipartFile[]"), WebModule::FileContent_getArray);
    
    /* End of FileContent class */

    /* PostContent class */
    /**
     * class PostContent
     * 
     * Holds POST content posted from a web client to your apache2 server
     */
    c = this->getModuleSystem()->getClassSystem()->registerClass("PostContent");
    /**
     * @class PostContent
     * Gets the POST content as a string for the POST field with the given name
     * function get(string index_name) : string
     */
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModulePOSTContentObject> post_obj = std::dynamic_pointer_cast<WebModulePOSTContentObject>(object);
        return_value->type = VALUE_TYPE_STRING;

        if (post_obj->content.find(arguments[0].svalue) == post_obj->content.end())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "", interpreter->getStackTraceLog());
        }

        return_value->svalue = post_obj->content[arguments[0].svalue];
    });
    /**
     * @class PostContent
     * Returns true if the PostContent has the POST field with the given name
     * function has(string index_name) : boolean
     */
    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModulePOSTContentObject> post_obj = std::dynamic_pointer_cast<WebModulePOSTContentObject>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = (post_obj->content.find(arguments[0].svalue) != post_obj->content.end());
    });
    /* End of POSTContent class */

    /**
     * class Request
     * 
     * This is the Request class it is responsible for getting http request information from the web client
     * when running a marble script in apache2.
     * 
     * <b>You can access the object directly in a static way. For example Request.getMethod()</b>
     */
    c = this->getModuleSystem()->getClassSystem()->registerClass("Request");
    /**
     * @class Request
     * 
     * Returns the URI for this HTTP request
     * function getUri() : string
     */
    c->registerFunction("getUri", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->request_uri;
    });

    /**
     * @class Request
     * 
     * Returns the HTTP request method for this HTTP request
     * function getMethod() : string
     */
    c->registerFunction("getMethod", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->request_method;
    });

    /**
     * @class Request
     * 
     * Returns the "GET" request arguments for this HTTP request
     * fucntion getArguments() : RequestArguments
     */
    c->registerFunction("getArguments", {}, VarType::fromString("RequestArguments"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = web_mod_obj->request_arguments;
    });

    /**
     * @class Request
     * 
     * Returns the "POST" content for this HTTP request
     * 
     * function getContent() : PostContent
     */
    c->registerFunction("getContent", {}, VarType::fromString("PostContent"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = web_mod_obj->content;
    });

    /**
     * @class Request
     * Returns the cookie with the given name. Empty string is returned if no cookie was found.
     * function getCookie(string cookieName) : string
     */
    c->registerFunction("getCookie", {VarType::fromString("string")}, VarType::fromString("string"), WebModule::Request_getCookie);
    
    /**
     * @class Request
     * Returns the protocol such as HTTP or HTTPS
     * function getProtocol() : string
     */
    c->registerFunction("getProtocol", {}, VarType::fromString("string"), WebModule::Request_getProtocol);

    /**
     * @class Request
     * 
     * Returns the file upload content for this HTTP request
     * 
     * function getFileContent() : FileContent
     */
    c->registerFunction("getFileContent", {}, VarType::fromString("FileContent"), WebModule::Request_getFileContent);

    /**
     * @class Request
     * 
     * Returns the IP address of the person who is connected to your apache server issueing a request on this script
     * function getRequesterIP() : string
     */
    c->registerFunction("getRequesterIP", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->requester_ip;
    });


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

    std::shared_ptr<WebModule_ResponseObject> response_obj = std::dynamic_pointer_cast<WebModule_ResponseObject>(Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Response"), {}));
    root_scope->createVariable("Response", "Response", response_obj);

    // Set the default response code to 200 OK
    interpreter->properties["response_code"] = std::to_string(200);
}

void WebModule::parseForRequestObject(Scope* root_scope, Interpreter* interpreter, request_rec* req)
{
    Variable* request_variable = root_scope->getVariableAnyScope("Request");
  
    std::shared_ptr<WebModuleObject> object = std::dynamic_pointer_cast<WebModuleObject>(request_variable->value.ovalue);
    object->req = (struct request_rec*) req;
    object->request_uri = req->unparsed_uri;
    object->requester_ip = req->useragent_ip;
    object->request_method = req->method;
    object->request_arguments->arguments = parseGet(req);
    object->cookies = parseCookies(req);
    object->protocol = req->protocol;

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
       object->file_content->content_array = mparse.file_field_content_array;
    }

}

void WebModule::parseForResponseObject(Scope* root_scope, Interpreter* interpreter, request_rec* req)
{
    // Let's setup the Response object essentials so that it can work correctly
    Variable* response_variable = root_scope->getVariableAnyScope("Response");
    std::shared_ptr<WebModule_ResponseObject> response_obj = std::dynamic_pointer_cast<WebModule_ResponseObject>(response_variable->value.ovalue);
    response_obj->req = req;
}

void WebModule::parseRequest(Interpreter* interpreter, request_rec* req)
{
    Scope* root_scope = interpreter->getRootScope();
    parseForRequestObject(root_scope, interpreter, req);
    parseForResponseObject(root_scope, interpreter, req);
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

    
        std::string data = readNextInBuffer(end, &pos, "\r\n" + signature);
        std::string field_name = content_disposition_properties["name"];
        if (!is_file)
        {
            // This is not a file so lets add this to our post field content
            p.post_field_content[field_name] = data;
        }
        else
        {
            std::string filename = content_disposition_properties["filename"];
            if (filename != "")
            {
                std::shared_ptr<MultipartFileObject> file_obj = std::dynamic_pointer_cast<MultipartFileObject>(Object::create(interpreter->getClassSystem()->getClassByName("MultipartFile")));
                file_obj->name = filename;
                file_obj->type = header_info_map["content-type"].begin()->first;
                
                // Let's get the file extension if there is one
                std::vector<std::string> name_split = str_split(filename, ".");
                if (name_split.size() > 1)
                {
                    file_obj->ext = name_split[name_split.size()-1];
                }

                // Let's write this file to temp
                file_obj->path = writeTemp(data.c_str(), data.size());
                if (field_name.find("[]") != std::string::npos)
                {
                    // This is an array content
                    std::string real_field_name = str_split(field_name, "[]")[0];
                    p.file_field_content_array[real_field_name].push_back(file_obj);
                }
                else
                {
                    p.file_field_content[field_name] = file_obj;
                }
            }
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

std::map<std::string, std::string> WebModule::parseCookies(request_rec* req)
{
    std::map<std::string, std::string> result;
    const char* cookies = apr_table_get(req->headers_in, "Cookie");
    if (cookies)
    {
        // We have cookies lets parse them
        result = parseKeyAndValueForString(cookies);
    }

    return result;
}

void WebModule::Request_getFileContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleObject> wm_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
    return_value->set(wm_obj->file_content);
}

void WebModule::Request_getCookie(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleObject> wm_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
    if(wm_obj->cookies.find(values[0].svalue) != wm_obj->cookies.end())
    {
        return_value->set(wm_obj->cookies[values[0].svalue]);
        return;
    }

    return_value->set("");
}

void WebModule::Request_getProtocol(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleObject> wm_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
    return_value->set(wm_obj->protocol);
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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "", interpreter->getStackTraceLog());
    }

    return_value->set(file_content_obj->content[values[0].svalue]);
}

void WebModule::FileContent_getArray(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModulePOSTFileContentObject> file_content_obj = std::dynamic_pointer_cast<WebModulePOSTFileContentObject>(object);
    if(file_content_obj->content_array.find(values[0].svalue) == file_content_obj->content_array.end())
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "", interpreter->getStackTraceLog());
    }

    std::vector<std::shared_ptr<MultipartFileObject>> file_array_list = file_content_obj->content_array[values[0].svalue];
    Variable* variables = new Variable[file_array_list.size()];
    for(int i = 0; i < file_array_list.size(); i++)
    {
        std::shared_ptr<MultipartFileObject> file_obj = file_array_list.at(i);
        Variable* var = &variables[i];
        var->type = VARIABLE_TYPE_OBJECT;
        var->value.holder = var;
        var->value.set(file_obj);
    }

    return_value->type = VALUE_TYPE_ARRAY;
    return_value->set(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, file_array_list.size()));
}


void WebModule::FileContent_hasArray(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
     std::shared_ptr<WebModulePOSTFileContentObject> file_content_obj = std::dynamic_pointer_cast<WebModulePOSTFileContentObject>(object);
     return_value->set(file_content_obj->content_array.find(values[0].svalue) != file_content_obj->content_array.end());
}
