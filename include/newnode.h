#ifndef NEWNODE_H
#define NEWNODE_H

#include "einode.h"
#include <vector>
class ArrayNode;
class Interpreter;
class NewNode : public ExpressionInterpretableNode
{
    public:
        NewNode();
        virtual ~NewNode();
        ExpressionInterpretableNode* type_node;
        bool isArray();
        std::vector<ExpressionInterpretableNode*> array_values;
        virtual Value interpret(Interpreter* interpreter);
    private:
        std::shared_ptr<Array> new_array_array(Interpreter* interpreter, int total_elements, std::vector<ExpressionInterpretableNode*>::iterator it);
        std::shared_ptr<Array> new_variable_array(int var_type, int total_elements);
        void handle_array(Interpreter* interpreter, Value& v, std::vector<ExpressionInterpretableNode*>::iterator it);
};
#endif
