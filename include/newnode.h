#ifndef NEWNODE_H
#define NEWNODE_H

#include "einode.h"
class NewNode : public ExpressionInterpretableNode
{
    public:
        NewNode();
        virtual ~NewNode();
        ExpressionInterpretableNode* exp;
        virtual Value interpret(Interpreter* interpreter);
};
#endif
