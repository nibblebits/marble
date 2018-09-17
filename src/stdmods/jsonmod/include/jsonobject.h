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

#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include "object.h"
#include <jsoncpp/json/json.h>
#include <map>
#include <string>


class ModuleSystem;
class JsonObject : public Object
{
public:
    JsonObject(Class* c);
    virtual ~JsonObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Value getNonNamedValueForJSONValue(Interpreter* interpreter, Json::Value& json_value);
    static std::map<std::string, Value> getSystemValuesForJSONValue(Interpreter* interpreter, Json::Value& values);
    static std::string parseValueToJson(Value& value);
    static std::string parseMapToJson(std::map<std::string, Value> map);
    
    static void registerClass(ModuleSystem* moduleSystem);

    // Native Json methods
    static void Json_decode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Json_encode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif