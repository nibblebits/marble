#ifndef INCLUDEONCENODE_H
#define INCLUDEONCENODE_H
#include "inode.h"
class ExpressionInterpretableNode;
class IncludeOnceNode : public InterpretableNode
{
public:
    IncludeOnceNode();
    virtual ~IncludeOnceNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);

    ExpressionInterpretableNode* exp;
};
#endif
