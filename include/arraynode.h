#ifndef ARRAYNODE_H
#define ARRAYNODE_H
#include "einode.h"
class ArrayNode : public ExpressionInterpretableNode
{
public:
    ArrayNode();
    virtual ~ArrayNode();
    ExpressionInterpretableNode* index_node;
    ExpressionInterpretableNode* next_element;
    
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};

#endif
