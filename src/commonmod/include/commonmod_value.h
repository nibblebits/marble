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

#ifndef COMMONMODULE_VALUE
#define COMMONMODULE_VALUE
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
/**
 * The Value class is almost like the Value class of the Interpreter holding either a string, number, boolean or Object
 */

class Value;
class CommonModule_Value : public Object
{
public:
    CommonModule_Value(Class* c);
    virtual ~CommonModule_Value();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    static void Value_AssignmentSet(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Value_Set(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Value_GetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Value_GetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Value_GetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    Value value;

};


#endif