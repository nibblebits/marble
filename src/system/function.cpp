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

#include "function.h"
#include "interpreter.h"
#include "writtenfunction.h"
#include "nodes.h"
#include "token.h"
#include "posinfo.h"
#include "systemhandler.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptions/systemexception.h"

Function::Function(FUNCTION_TYPE type, std::string name)
{
    this->type = type;
    this->name = name;
    this->is_pure = false;
    this->access = MODIFIER_ACCESS_PUBLIC;
    this->cls = NULL;
    this->is_operator_overloading = false;
}

Function::~Function()
{
}

void Function::invoke(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    Value fake_value;
    if (return_value == NULL)
    {
        return_value = &fake_value;
    }

    FunctionSystem *f_system = interpreter->getFunctionSystem();
    // We are also in a function
    f_system->setCurrentFunction(this);

    // Lets keep a stack trace of this function call
    std::string address = (object != NULL ? object->getClass()->name : "Global") + "." + this->name;
    PosInfo posInfo;
    if (this->type == FUNCTION_TYPE_WRITTEN)
    {
        /* This is a written function lets get the interpreters last function call node's position.
         * if there is one. As this function may have been called natively*/
        FunctionCallNode *fc_node = interpreter->getLastFunctionCallNode();
        if (fc_node != NULL)
            posInfo = fc_node->posInfo;
    }

    interpreter->addToStackTrace(address, posInfo);
    // Lets create a new parented scope for any extras before the invoke
    interpreter->new_parented_scope();
    try
    {
        /**
         * Before invoking the function we must tell the interpreter the current caller permissions.
         * However if they are calling "getCallerPermissions" we should not set the permissions 
         * as this will result in it only ever bringing back its own permissions
         */
        if (this->name != "getCallerPermissions")
            interpreter->setCallerPermissions(caller_scope->permissions);
        this->invoke_impl(interpreter, values, return_value, object, caller_scope);
        if (this->name != "getCallerPermissions")
            interpreter->finishCallerPermissions();
    }
    catch (SystemException &ex)
    {
        // An exception was thrown! Lets pop us off the stack trace and rethrow it
        interpreter->popFromStackTrace();

        // We are done with our function
        f_system->finishCurrentFunction();

        // Let's finish the parented scope that we created
        interpreter->finish_parented_scope();

        // Finish the caller permissions
        if (this->name != "getCallerPermissions")
            interpreter->finishCallerPermissions();

        throw ex;
    }

    // Now lets pop it off
    interpreter->popFromStackTrace();

    // We are done with our function
    f_system->finishCurrentFunction();

    // Let's finish the parented scope that we created
    interpreter->finish_parented_scope();
}

std::string Function::getName()
{
    return this->name;
}

void Function::Blank(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // An empty static function used for when a native programmer wants to create a function that points to nothing.
}