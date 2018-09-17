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

#ifndef COMMONMODULE_INPUTSTREAM
#define COMMONMODULE_INPUTSTREAM
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
#include <deque>
class CommonModule_InputStream : public Object
{
public:
    CommonModule_InputStream(Class* c);
    virtual ~CommonModule_InputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    // A char buffer for the input stream
    std::deque<char> buffer;

    // The amount to load into the buffer when calling the fill function
    int amount_to_fill;

    // Native InputStream functions
    static void InputStream_Put(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void InputStream_Read(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void InputStream_Fill(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};


#endif