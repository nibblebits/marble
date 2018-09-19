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

#include "commonmod_map.h"
#include "commonmod_value.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "misc.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>

CommonModule_Map::CommonModule_Map(Class* c) : Object(c)
{

}
CommonModule_Map::~CommonModule_Map()
{

}
std::shared_ptr<Object> CommonModule_Map::newInstance(Class* c)
{
    return std::make_shared<CommonModule_Map>(c);
}

Class* CommonModule_Map::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class Map
     * 
     * The Map class provides map functionality and allows Value's to be set or accessed through an index name
     */ 
    Class* c = moduleSystem->getClassSystem()->registerClass("Map");
    c->setDescriptorObject(std::make_shared<CommonModule_Map>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class Map
     * function set(string index, Value value) : void
     * 
     * Sets the index value represented by the index to the new provided value
     */
    c->registerFunction("set", {VarType::fromString("string"), VarType::fromString("Value")}, VarType::fromString("void"), CommonModule_Map::Map_Set);

    /**
     * @class Map
     * function set(string index, number value) : void
     * 
     * Sets the index value represented by the index to the new provided value
     */
    c->registerFunction("set", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("void"), CommonModule_Map::Map_Set);

    /**
     * @class Map
     * function set(string index, string value) : void
     * 
     * Sets the index value represented by the index to the new provided value
     */
    c->registerFunction("set", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), CommonModule_Map::Map_Set);



    /**
     * @class Map
     * function get(string index) : Value
     * 
     * Gets the Value from this map that has the index name that is equal to the index provided
     */
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("Value"), CommonModule_Map::Map_Get);
}

void CommonModule_Map::Map_Set(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // If our value type is a string or number then we are going to have to create a Value object for it
    // and set the Value's value to this string or number
    if (values[1].type == VALUE_TYPE_STRING || values[0].type == VALUE_TYPE_NUMBER)
    {
        std::shared_ptr<CommonModule_Value> new_value = 
                    std::dynamic_pointer_cast<CommonModule_Value>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Value"), {values[0]}));
        // Overwrite our value passed with the new object Value
        values[1] = Value(new_value);
    }

    std::shared_ptr<CommonModule_Map> map_obj = std::dynamic_pointer_cast<CommonModule_Map>(object);
    map_obj->value_map[values[0].svalue] = std::make_unique<Value>(&values[1]);
}

void CommonModule_Map::Map_Get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Map> map_obj = std::dynamic_pointer_cast<CommonModule_Map>(object);
    if (map_obj->value_map.find(values[0].svalue) == map_obj->value_map.end())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "This map does not contain the key \"" + values[0].svalue + "\"", interpreter->getStackTraceLog());

    Value* value_map_value = map_obj->value_map[values[0].svalue].get();
    Value return_val = Value(value_map_value);
    return_value->set(&return_val);
}