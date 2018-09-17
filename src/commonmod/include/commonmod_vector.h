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

#ifndef COMMONMODULE_VECTOR
#define COMMONMODULE_VECTOR
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
/**
 * The Vector class extends the List class and implements Vector functionality such as pushing and popping
 */
class Value;
class CommonModule_Vector : public Object
{
public:
    CommonModule_Vector(Class* c);
    virtual ~CommonModule_Vector();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    static void Vector_Push(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Vector_Pop(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Vector_Back(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Vector_Get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Vector_isEmpty(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Vector_Size(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


    // The values of this Vector are stored here in this C++ vector
    std::vector<Value> vec_values;

};


#endif