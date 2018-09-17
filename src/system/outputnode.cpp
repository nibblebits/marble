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

#include "outputnode.h"
#include "interpreter.h"
#include "validator.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"

OutputNode::OutputNode() : ExpressionInterpretableNode(NODE_TYPE_OUTPUT_NODE)
{

}

OutputNode::~OutputNode()
{

}

void OutputNode::test(Validator* validator, struct extras extra)
{
    this->exp->test(validator);
}

Value OutputNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v = this->exp->interpret(interpreter, extra);
    if (v.type != VALUE_TYPE_STRING)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidTypeException"), {})), "Direct output is only allowed for strings. You should cast this", interpreter->getStackTraceLog());
    }

    interpreter->output(v.svalue.c_str(), v.svalue.size());
    return v;
}   

void OutputNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}