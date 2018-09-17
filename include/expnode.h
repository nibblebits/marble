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

#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "statics.h"
#include "einode.h"
#include "evaluatingnode.h"
#include "validator.h"
class Interpreter;
class Value;
class ExpNode : public ExpressionInterpretableNode
{
public:
    ExpNode();
    virtual ~ExpNode();
    virtual Value interpret(Interpreter* intrerpreter, struct extras extra={});
    virtual void test(Validator* validator, struct extras extra={});
    void test_obj_access(Validator* validator, struct extras extra={});
    void test_assign(Validator* validator);
    void test_regular_exp(Validator* validator);
    /**
     * Tests logical expressions && and ||
     * a && b, a == 1 || b == 2
     */
    void test_logical_operator_exp(Validator* validator);
    void test_compare_expression(Validator* validator);

    bool checkOperatorOverloadPossible(Validator* validator, ExpressionInterpretableNode* left_node, ExpressionInterpretableNode* right_node, bool no_loop=false);
    Value objectCompareOperatorOverloadExecute(Value &value1, Value &value2, std::string op, Interpreter *interpreter, bool dont_flip=false);
    Value compareGetResult(Value& value1, Value& value2, std::string op, Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    
    ExpressionInterpretableNode* left;
    ExpressionInterpretableNode* right;
    std::string op;
    bool isAssignmentOperator();
   
private:
    Value mathify(Value& value1, Value& value2, std::string op, Interpreter* interpreter);
};
#endif
