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

#include "array.h"
#include "variable.h"
#include "interpreter.h"
#include <iostream>

Array::Array(Class *c) : Object(c)
{
    this->variables = NULL;
    this->count = 0;
}

Array::Array(Class *c, Variable *variables, int count) : Object(c)
{
    this->variables = variables;
    this->count = count;
}

Array::~Array()
{
    if (this->variables != NULL)
        delete[] variables;
}

void Array::setup()
{
    // Nothing to setup
}

Class *Array::registerClass(SystemHandler *systemHandler)
{
    /**
     * class array
     * 
     * An instance of this class is automatically created when creating new array variables.
     * For example "new string[10]"
     */

    Class *array_cls = systemHandler->getClassSystem()->registerClass("array");

    /**
     * @class Array
     * Gets the size of this array
     * function size() : number
     */
    array_cls->registerFunction("size", {}, VarType::fromString("number"), Array::Array_size);

    /**
     * @class Array
     * 
     * Converts this array into a string equivilant. If this array is a number array the bytes are returned as a non converted string
     * function toString() : string
     */
    array_cls->registerFunction("toString", {}, VarType::fromString("string"), Array::Array_toString);
}

void Array::Array_size(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = array->count;
}

void Array::Array_toString(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string result = "";
    std::shared_ptr<Array> array_obj = std::dynamic_pointer_cast<Array>(object);

    for (int i = 0; i < array_obj->count; i++)
    {
        Variable var = array_obj->variables[i];
        switch (var.type)
        {
        case VARIABLE_TYPE_NUMBER:
            result += (unsigned char)var.value.dvalue;
            break;
        }
    }
    return_value->set(result);
}