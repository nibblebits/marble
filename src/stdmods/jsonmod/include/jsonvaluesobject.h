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

#ifndef JSONVALUESOBJECT_H
#define JSONVALUESOBJECT_H

#include "object.h"
#include "value.h"
#include <map>
#include <string>

class ModuleSystem;
class Value;
class JsonValuesObject : public Object
{
public:
    JsonValuesObject(Class* c);
    virtual ~JsonValuesObject();

    static void registerClass(ModuleSystem* moduleSystem);
    virtual std::shared_ptr<Object> newInstance(Class* c);
    void ensureIndexExists(Interpreter* interpreter, std::string index_name);

    // The value for this session value object
    std::map<std::string, Value> values;

    // Native JsonValues functions/methods
    static void JsonValues_Has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_SetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_SetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_SetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    static void JsonValues_GetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_GetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_GetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_GetArray(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);

};

#endif