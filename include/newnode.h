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
