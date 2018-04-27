#ifndef BOOLEANLITERALNODE_H
#define BOOLEANLITERALNODE_H
#include "einode.h"
class BooleanLiteralNode : public ExpressionInterpretableNode
{
public:
    BooleanLiteralNode();
    virtual ~BooleanLiteralNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    bool literal;
};
#endif
