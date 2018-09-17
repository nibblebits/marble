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

#include "includeoncenode.h"
#include "einode.h"
#include "interpreter.h"
#include "validator.h"
#include "statics.h"
#include "filepermission.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "exceptions/IOException.h"
#include "object.h"
#include "misc.h"

#include <iostream>
IncludeOnceNode::IncludeOnceNode() : InterpretableNode(NODE_TYPE_INCLUDE_ONCE)
{

}

IncludeOnceNode::~IncludeOnceNode()
{

}

void IncludeOnceNode::test(Validator* validator, struct extras extra)
{
    validator->expecting("string");
    this->exp->test(validator);
    validator->endExpecting(); 

}

Value IncludeOnceNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v = this->exp->interpret(interpreter);

    std::string absolute_path = getAbsolutePath(v.svalue);
    // Before we do anything let's make sure we have permission to load this file
    FilePermission::checkPermissionAllows(interpreter, interpreter->getCurrentScope(), absolute_path.c_str(), "r");
    interpreter->new_parented_scope();
    try
    {
        std::string script_address = v.svalue;
        if (interpreter->hasRunScript(script_address))
            return v;
        interpreter->runScript(script_address.c_str());
    }
    catch(IOException& e)
    {
        interpreter->finish_parented_scope();
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "", interpreter->getStackTraceLog());
    }
    catch(...)
    {
        interpreter->finish_parented_scope();
        throw;
    }

    interpreter->finish_parented_scope();
    return v;
}

void IncludeOnceNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}
