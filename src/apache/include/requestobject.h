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

#ifndef WEBMODULE_REQUESTOBJECT
#define WEBMODULE_REQUESTOBJECT
#include "object.h"
#include "module.h"
#include "class.h"

#include "httpd.h"
#include "apache_mod.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"

#include <stdio.h>
#include <string>
#include <memory>
#include <map>

class WebModulePOSTContentObject;
class WebModulePOSTFileContentObject;
class WebModuleRequestArgumentsObject;
class RequestObject : public Object
{
public:
    RequestObject(Class* c);
    virtual ~RequestObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    /**
     *
     * Registers the Request class
     */
    static Class* registerClass(ModuleSystem* moduleSystem);

    // The given apache request object
    request_rec* req;
    
    std::string request_uri;
    std::string requester_ip;
    std::string request_method;
    std::string protocol;
    std::shared_ptr<WebModulePOSTContentObject> content;
    std::shared_ptr<WebModulePOSTFileContentObject> file_content;
    std::shared_ptr<WebModuleRequestArgumentsObject> request_arguments;
    // Cookies that have been read from the client
    std::map<std::string, std::string> cookies;
    // Headers that have been read from the client
    std::map<std::string, std::string> headers;

    // Static Native Request functions
    static void Request_getUri(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getMethod(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getArguments(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getFileContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getCookie(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getProtocol(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getHeader(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};


#endif