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

#ifndef FUNCTIONNODE_H
#define FUNCTIONNODE_H
#include "einode.h"
#include <string>
#include <vector>

class VarNode;
class BodyNode;
class FunctionNode : public ExpressionInterpretableNode
{
public:
    FunctionNode();
    virtual ~FunctionNode();
    virtual void test(Validator* validator, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, struct extras extra={});
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    bool isArray();
    
    std::string name;
    struct BodyNode* body;
    std::vector<VarNode*> args;
    bool is_pure;
    ExpressionInterpretableNode* return_type;
    int dimensions;


    /**
    * is_operator_overloading should be true if the function is an overloaded operator
    * the overloaded_operator string should be set to the operator that is overloaded
    */
    bool is_operator_overloading;
    std::string overloaded_operator;



    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
};
#endif
