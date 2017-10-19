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
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
private:
};
#endif
