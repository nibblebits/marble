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

#include "stringnode.h"
#include "validator.h"
#include "exceptions/testerror.h"

StringNode::StringNode() : ExpressionInterpretableNode(NODE_TYPE_STRING)
{

}

StringNode::~StringNode()
{

}


void StringNode::test(Validator* validator, struct extras extra)
{
    if (!validator->isExpecting())
        return;
    VALUE_TYPE expecting_type = validator->getExpectingValueType();
    if (expecting_type != VALUE_TYPE_STRING)
    {
        throw TestError("a string was provided");
    }

}

Value StringNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value result;
    result.type = VALUE_TYPE_STRING;
    result.svalue = this->value;
    return result;
}

void StringNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        // Strings default to the STRING datatype.
        evaluation->datatype.type = VARIABLE_TYPE_STRING;
        evaluation->datatype.value = "string";
    }
}
