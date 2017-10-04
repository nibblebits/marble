#ifndef LITERALNODE_H
#define LITERALNODE_H

#include "einode.h"
class Validator;
class LiteralNode : public ExpressionInterpretableNode
{
public:
    LiteralNode();
    virtual ~LiteralNode();
    virtual void test(Validator* validator);
    double value;
    virtual Value interpret(Interpreter* interpreter);
private:
};
#endif
