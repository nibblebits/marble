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

#ifndef CURLOBJECT_H
#define CURLOBJECT_H

#include "object.h"
#include <curl/curl.h>

class ModuleSystem;
class Interpreter;
class CurlObject : public Object
{
public:
    CurlObject(Class* c);
    virtual ~CurlObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);    
    static void newInterpreter(Interpreter* interpreter);

    CURL* curl;

    /**
     *  These are curl lists created that need to be freed when we execute
     */
    std::vector<struct curl_slist*> lists_to_free;

    /**
     * This is where data is written to after an execution
     */
    std::string write_data;


    /**
     * Unfortunetly due to Curl being a C library we cannot use std::string.
     * The way std::string works is that it frees the memory when it leaves scope and because of this
     * the byte data of std::string needs to be copied so that it is still available when we execute Curl.
     * We will store the pointers in here to delete later
     */
    std::vector<const char*> strings_to_delete;

    // Native CurlObject methods
    static void Curl_setopt(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Curl_execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Curl_close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Curl_getinfo(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};
#endif