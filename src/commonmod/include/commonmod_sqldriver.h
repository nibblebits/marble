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

#ifndef COMMONMOD_SQLDRIVER
#define COMMONMOD_SQLDRIVER

#include "object.h"
#include <vector>


/**
 * An abstract pure SQLDriver class. Extend this if you wish to create your own SQL driver
 */

class ModuleSystem;
class Interpreter;
class CommonModule_SqlDriver : public Object
{
public:
    CommonModule_SqlDriver(Class* c);
    virtual ~CommonModule_SqlDriver();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    static void newInterpreter(Interpreter* interpreter);

    // Native SQLDriver functions
    static void SQLDriver_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLDriver_registerDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLDriver_getDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLDriver_hasDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
private:

};

#endif