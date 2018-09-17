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

#include "../include/shellobject.h"
#include "../include/shellexecutionobject.h"
#include "../include/shellpermission.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "misc.h"
#include <iostream>
#include <thread>

ShellObject::ShellObject(Class* c) : Object(c)
{

}

ShellObject::~ShellObject()
{
        
}

std::shared_ptr<Object> ShellObject::newInstance(Class* c)
{
    return std::make_shared<ShellObject>(c);
}

void ShellObject::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class Shell
     * 
     * Responsible for executing shell commands
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Shell");
    c->setDescriptorObject(std::make_shared<ShellObject>(c));

    /**
     * @class ShellObject
     * 
     * Constructs this ShellObject object. 
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class ShellObject
     * 
     * Executes the provided command and returns a ShellExecution object that holds execution state and responses.
     * 
     * You are required to hold a ShellPermision permission in order to execute shell commands
     * 
     * function execute(string command) : ShellExecution
     */
     c->registerFunction("execute", {VarType::fromString("string")}, VarType::fromString("ShellExecution"), ShellObject::Shell_execute);
     
      
}

void ShellObject::Shell_execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Ensure we have permission
    ShellPermission::ensurePermission(interpreter, caller_scope);
    
    // Create the ShellExecution object
    std::shared_ptr<ShellExecutionObject> shell_exec_obj = std::dynamic_pointer_cast<ShellExecutionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("ShellExecution"), {}));
    int exec_return_val = 0;
    std::string response = exec(values[0].svalue, exec_return_val);
    shell_exec_obj->response_code = exec_return_val;
    shell_exec_obj->response = response; 
    return_value->set(shell_exec_obj);
}