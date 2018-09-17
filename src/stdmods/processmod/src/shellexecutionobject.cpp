
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

#include "../include/shellexecutionobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "misc.h"
#include <iostream>
#include <thread>

ShellExecutionObject::ShellExecutionObject(Class* c) : Object(c)
{

}

ShellExecutionObject::~ShellExecutionObject()
{
        
}

std::shared_ptr<Object> ShellExecutionObject::newInstance(Class* c)
{
    return std::make_shared<ShellExecutionObject>(c);
}

void ShellExecutionObject::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class ShellExecution
     * 
     * An instance of this class is returned when you have created a shell execution. This class holds
     * the response code from the shell you executed along with the response body
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("ShellExecution");
    c->setDescriptorObject(std::make_shared<ShellExecutionObject>(c));

    /**
     * @class ShellExecution
     * 
     * Constructs this ShellExecution object. 
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class ShellExecution
     * 
     * Returns the response code for this ShellExecution
     * 
     * function getResponseCode() : number
     */
     c->registerFunction("getResponseCode", {}, VarType::fromString("number"), ShellExecutionObject::getResponseCode);
     
    /**
     * @class ShellExecution
     * 
     * Returns the response body for this ShellExecution
     * 
     * function getResponse() : string
     */
     c->registerFunction("getResponse", {}, VarType::fromString("string"), ShellExecutionObject::getResponse);
     
}

void ShellExecutionObject::getResponseCode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<ShellExecutionObject> shell_exec_obj = std::dynamic_pointer_cast<ShellExecutionObject>(object);
    return_value->set(shell_exec_obj->response_code);   
}

void ShellExecutionObject::getResponse(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<ShellExecutionObject> shell_exec_obj = std::dynamic_pointer_cast<ShellExecutionObject>(object);
    return_value->set(shell_exec_obj->response);   
}
