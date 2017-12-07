#ifndef BREAKNODE_H
#define BREAKNODE_H
#include "inode.h"
class ExpressionInterpretableNode;
class BreakNode : public InterpretableNode
{
public:
    BreakNode();
    virtual ~BreakNode();
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};
#endif
