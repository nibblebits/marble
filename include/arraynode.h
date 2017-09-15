#ifndef ARRAYNODE_H
#define ARRAYNODE_H
#include "einode.h"
class ArrayNode : public ExpressionInterpretableNode
{
public:
    ArrayNode();
    virtual ~ArrayNode();
    ExpressionInterpretableNode* index_node;
    Node* next_element;
    
    virtual Value interpret(Interpreter* interpreter);
};

#endif
