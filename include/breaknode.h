#ifndef BREAKNODE_H
#define BREAKNODE_H
#include "inode.h"
class ExpressionInterpretableNode;
class BreakNode : public InterpretableNode
{
public:
    BreakNode();
    virtual ~BreakNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};
#endif
