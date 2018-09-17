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

#ifndef NEWNODE_H
#define NEWNODE_H

#include "einode.h"
#include <vector>
class ArrayNode;
class Interpreter;
class Validator;
class FunctionCallNode;
class NewNode : public ExpressionInterpretableNode
{
    public:
        NewNode();
        virtual ~NewNode();
        ExpressionInterpretableNode* type_node;
        bool isArray();
        virtual void test(Validator* validator, struct extras extra);
        virtual Value interpret(Interpreter* interpreter, struct extras extra);
        virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
        std::vector<ExpressionInterpretableNode*> array_values;
    private:
        void test_for_object(Validator* validator);
        void test_for_array(Validator* validator);
        std::shared_ptr<Array> new_array_array(Interpreter* interpreter, int total_elements, std::vector<ExpressionInterpretableNode*>::iterator it);
        std::shared_ptr<Array> new_variable_array(Interpreter* interpreter, int var_type, int total_elements);
        void new_object_variable(Interpreter* interpreter, Value& v, FunctionCallNode* fc_node);
        void handle_new_variable(Interpreter* interpreter, Value& v);
        void handle_array(Interpreter* interpreter, Value& v, std::vector<ExpressionInterpretableNode*>::iterator it);
};
#endif
