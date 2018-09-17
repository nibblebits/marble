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

#ifndef SHELLEXECUTIONOBJECT_H
#define SHELLEXECUTIONOBJECT_H

#include "object.h"
class ModuleSystem;
class ShellExecutionObject : public Object
{
public:
    ShellExecutionObject(Class* c);
    virtual ~ShellExecutionObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);

    // The response code from this ShellExecution
    int response_code;

    // The response from this ShellExecution
    std::string response;

    // Native ShellExecution methods
    static void getResponseCode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void getResponse(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif