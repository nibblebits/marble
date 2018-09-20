
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
#include "requestobject.h"
#include "webmod.h"
#include "apache_mod.h"
#include "cookiepermission.h"
#include "headerpermission.h"
#include "requestargumentsobject.h"

#include "exceptionobject.h"
#include "exceptions/systemexception.h"

RequestObject::RequestObject(Class *c) : Object(c)
{
}

RequestObject::~RequestObject()
{
}

std::shared_ptr<Object> RequestObject::newInstance(Class *c)
{
    return std::make_shared<RequestObject>(c);
}

Class* RequestObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Request
     * 
     * This is the Request class it is responsible for getting http request information from the web client
     * when running a marble script in apache2.
     * 
     * <b>You can access the object directly in a static way. For example Request.getMethod()</b>
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("Request");
    c->setDescriptorObject(std::make_shared<RequestObject>(c));
    /**
     * @class Request
     * 
     * Returns the URI for this HTTP request
     * function getUri() : string
     */
    c->registerFunction("getUri", {}, VarType::fromString("string"), RequestObject::Request_getUri);

    /**
     * @class Request
     * 
     * Returns the HTTP request method for this HTTP request
     * function getMethod() : string
     */
    c->registerFunction("getMethod", {}, VarType::fromString("string"), RequestObject::Request_getMethod);

    /**
     * @class Request
     * 
     * Returns the "GET" request arguments for this HTTP request
     * fucntion getArguments() : RequestArguments
     */
    c->registerFunction("getArguments", {}, VarType::fromString("RequestArguments"), RequestObject::Request_getArguments);

    /**
     * @class Request
     * 
     * Returns the "POST" content for this HTTP request
     * 
     * function getContent() : PostContent
     */
    c->registerFunction("getContent", {}, VarType::fromString("PostContent"), RequestObject::Request_getContent);

    /**
     * @class Request
     * Returns the cookie with the given name. Empty string is returned if no cookie was found.
     * function getCookie(string cookieName) : string
     */
    c->registerFunction("getCookie", {VarType::fromString("string")}, VarType::fromString("string"), RequestObject::Request_getCookie);
    
     /**
     * @class Request
     * Returns the header value with the given name for the header sent from the web client to the web server.
     * function getHeader(string header_name) : string
     */
    c->registerFunction("getHeader", {VarType::fromString("string")}, VarType::fromString("string"), RequestObject::Request_getHeader);
    
    
    /**
     * @class Request
     * Returns the protocol such as HTTP or HTTPS
     * function getProtocol() : string
     */
    c->registerFunction("getProtocol", {}, VarType::fromString("string"), RequestObject::Request_getProtocol);

    /**
     * @class Request
     * 
     * Returns the file upload content for this HTTP request
     * 
     * function getFileContent() : FileContent
     */
    c->registerFunction("getFileContent", {}, VarType::fromString("FileContent"), RequestObject::Request_getFileContent);

    /**
     * @class Request
     * 
     * Returns the IP address of the person who is connected to your apache server issueing a request on this script
     * function getRequesterIP() : string
     */
    c->registerFunction("getRequesterIP", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = req_obj->requester_ip;
    });

}



void RequestObject::Request_getUri(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = req_obj->request_uri;
}

void RequestObject::Request_getMethod(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = req_obj->request_method;
}

void RequestObject::Request_getArguments(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    return_value->type = VALUE_TYPE_OBJECT;
    return_value->ovalue = req_obj->request_arguments;
}

void RequestObject::Request_getContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    return_value->type = VALUE_TYPE_OBJECT;
    return_value->ovalue = req_obj->content;
}

void RequestObject::Request_getFileContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    return_value->set(req_obj->file_content);
}

void RequestObject::Request_getCookie(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    /*
     * Ensure the calling scope has read access for cookies
     */
    CookiePermission::ensureCookieReadAccess(interpreter, caller_scope);
    
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    if(req_obj->cookies.find(values[0].svalue) != req_obj->cookies.end())
    {
        return_value->set(req_obj->cookies[values[0].svalue]);
        return;
    }

    return_value->set("");
}

void RequestObject::Request_getHeader(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    /*
     * Ensure the calling scope has header read access or do not allow them to get headers sent to this web server
     */ 
    HeaderPermission::ensureHeaderReadAccess(interpreter, caller_scope);
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    if(req_obj->headers.find(values[0].svalue) != req_obj->headers.end())
    {
        return_value->set(req_obj->headers[values[0].svalue]);
        return;
    }

    return_value->set("");
}

void RequestObject::Request_getProtocol(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<RequestObject> req_obj = std::dynamic_pointer_cast<RequestObject>(object);
    return_value->set(req_obj->protocol);
}

