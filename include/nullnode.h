#ifndef NULLNODE_H
#define NULLNODE_H
#include "einode.h"
class NullNode : public ExpressionInterpretableNode
{
public:
    NullNode();
    virtual ~NullNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};
#endif
