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

#ifndef COMMONMODULE_OUTPUTSTREAM
#define COMMONMODULE_OUTPUTSTREAM
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
class CommonModule_OutputStream : public Object
{
public:
    CommonModule_OutputStream(Class* c);
    virtual ~CommonModule_OutputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    // A char buffer for the output stream
    std::vector<char> buffer;

    // The amount of bytes that must be in the buffer to flush automatically
    int amount_to_auto_flush;

    // Native OutputStream functions
    static void OutputStream_Write(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Get(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Empty(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Size(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Flush(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Print(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};


#endif