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

#ifndef WEBMODULE_RESPONSEOBJECT
#define WEBMODULE_RESPONSEOBJECT
#include "object.h"
#include "httpd.h"
#include "apache_mod.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"
#include "module.h"
#include "class.h"
#include <stdio.h>
#include <string>
#include <memory>
class WebModule_ResponseObject : public Object
{
public:
    WebModule_ResponseObject(Class* c);
    virtual ~WebModule_ResponseObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    /**
     * Checks to see if the WebModule_ResponseObject is free from errors
     * throws an exception if there are problems
     */
    static void errorCheck(std::shared_ptr<WebModule_ResponseObject> obj);

    // Native ResponseObject Marble Methods
    static void Response_setHeader(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Response_setCookie(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Response_setResponseCode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    // The given apache request object
    request_rec* req;

};


#endif