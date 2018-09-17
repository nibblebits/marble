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

#include "thrownode.h"
#include "einode.h"
#include "validator.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "exceptions/testerror.h"
#include <iostream>
ThrowNode::ThrowNode() : InterpretableNode(NODE_TYPE_THROW)
{
    this->exp = NULL;
}

ThrowNode::~ThrowNode()
{

}

void ThrowNode::test(Validator* validator, struct extras extra)
{
    try
    {
        validator->expecting("Exception");
        this->exp->test(validator);
        validator->endExpecting();
    }
    catch(TestError& ex)
    {
        throw TestError(ex.getMessage() + " while throwing");
    }
}

Value ThrowNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v = this->exp->interpret(interpreter);
    std::shared_ptr<ExceptionObject> exception_obj = std::dynamic_pointer_cast<ExceptionObject>(v.ovalue);
    if (exception_obj == NULL)
        throw std::logic_error("Throwing illegal exception that is not a native ExceptionObject");
    
    exception_obj->setStackLog(interpreter->getStackTraceLog());
    exception_obj->setThrowNode(this);

    // Lets throw this system exception
    throw SystemException(exception_obj);
    return v;
}

void ThrowNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of throw nodes is not supported");
}
