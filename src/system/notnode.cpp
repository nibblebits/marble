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

#include "notnode.h"
#include "nodes.h"
#include "statics.h"
NotNode::NotNode() : ExpressionInterpretableNode(NODE_TYPE_NOT)
{
    this->node = NULL;
}

NotNode::~NotNode()
{

}

void NotNode::test(Validator* validator, struct extras extra)
{
    validator->save();
    validator->expecting("boolean");
    this->node->test(validator);
    validator->endExpecting();
    validator->restore();
}

Value NotNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = !this->node->interpret(interpreter).dvalue;
    return v;
}

void NotNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        evaluation->datatype.type = VARIABLE_TYPE_BOOLEAN;
        evaluation->datatype.value = "boolean"; 
    }}
