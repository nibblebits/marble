#ifndef LITERALNODE_H
#define LITERALNODE_H

#include "einode.h"
class LiteralNode : public ExpressionInterpretableNode
{
public:
    LiteralNode();
    virtual ~LiteralNode();
    double value;
    virtual Value interpret(Interpreter* interpreter);
private:
};
#endif
