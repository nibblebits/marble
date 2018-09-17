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

#include "nullnode.h"
#include "exceptions/testerror.h"
#include "validator.h"
#include "interpreter.h"

NullNode::NullNode() : ExpressionInterpretableNode(NODE_TYPE_NULL)
{
}

NullNode::~NullNode()
{

}

void NullNode::test(Validator* validator, struct extras extra)
{
    if (!validator->isExpecting())
        return;
    
    // We now need to check if the type is valid
    VARIABLE_TYPE expecting_type = validator->getExpectingVariableType();
    if (expecting_type != VARIABLE_TYPE_OBJECT || validator->isExpectingArray())
        throw TestError("Attempting to use null on non object type");
}

Value NullNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.type = VALUE_TYPE_OBJECT;
    v.ovalue = NULL;
    return v;   
}

void NullNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // For evaluations we will just provide a datatype pointing to Object.
    evaluation->datatype.type = VARIABLE_TYPE_OBJECT;
    evaluation->datatype.value = "Object";
}
