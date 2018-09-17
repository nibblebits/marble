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

#include "booleanliteralnode.h"
#include "variable.h"
#include "exceptions/testerror.h"
#include "validator.h"
BooleanLiteralNode::BooleanLiteralNode() : ExpressionInterpretableNode(NODE_TYPE_BOOLEAN_LITERAL)
{
    this->literal = false;
}

BooleanLiteralNode::~BooleanLiteralNode()
{

}

void BooleanLiteralNode::test(Validator* validator, struct extras extra)
{
    if (validator->isExpecting() && validator->getExpectingType() != "boolean")
    {
        throw TestError("but a boolean literal was provided");
    }
}

Value BooleanLiteralNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.set(this->literal);
    return v;
}

void BooleanLiteralNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    // The keyword node only evaluates data types.
    if (!(expected_evaluation & EVALUATION_TYPE_DATATYPE))
        return;
    
        
    evaluation->type |= EVALUATION_TYPE_DATATYPE;
    evaluation->datatype.type = Variable::getVariableTypeForString("boolean");
    evaluation->datatype.value = (this->literal ? "true" : "false");
}