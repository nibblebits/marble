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

#ifndef FILEMOD_DIRECTORY
#define FILEMOD_DIRECTORY

#include <memory>
#include <string>
#include <stdio.h>
#include "filepermission.h"
#include "object.h"
#include "module.h"


/**
 * Responsible for directory related conditions such as creating and deleting directories
 */
class Interpreter;
class FileModule_Directory : public Object
{
public:
    FileModule_Directory(Class* c);
    virtual ~FileModule_Directory();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);


    /**
     * Called when a new interpreter is running
     */
    static void newInterpreter(Interpreter* interpreter);


private:
    // Native Directory functions/methods
    static void Directory_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Directory_List(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Directory_Delete(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif