#ifndef NEGNODE_H
#define NEGNODE_H

#include "einode.h"
class NegNode : public ExpressionInterpretableNode
{
    public:
        NegNode();
        virtual ~NegNode();
        ExpressionInterpretableNode* node;
        virtual Value interpret(Interpreter* interpreter);
};
#endif
