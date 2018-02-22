#ifndef CONTINUENODE_H
#define CONTINUENODE_H
#include "inode.h"
class ExpressionInterpretableNode;
class ContinueNode : public InterpretableNode
{
public:
    ContinueNode();
    virtual ~ContinueNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
};
#endif
