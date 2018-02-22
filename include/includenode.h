#ifndef INCLUDENODE_H
#define INCLUDENODE_H
#include "inode.h"
class ExpressionInterpretableNode;
class IncludeNode : public InterpretableNode
{
public:
    IncludeNode();
    virtual ~IncludeNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);

    ExpressionInterpretableNode* exp;
};
#endif
