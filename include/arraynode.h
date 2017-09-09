#ifndef ARRAYNODE_H
#define ARRAYNODE_H
#include "einode.h"
class ArrayNode : public ExpressionInterpretableNode
{
public:
    ArrayNode();
    virtual ~ArrayNode();
    ExpressionInterpretableNode* node;
    ExpressionInterpretableNode* next_element;
    
    virtual Value interpret(Interpreter* intrerpreter);
};

#endif
