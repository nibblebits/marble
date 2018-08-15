#ifndef DOWHILENODE_H
#define DOWHILENODE_H
#include "inode.h"
class BodyNode;
class ExpressionInterpretableNode;
class DoWhileNode : public InterpretableNode
{
public:
    DoWhileNode();
    virtual ~DoWhileNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ExpressionInterpretableNode* exp;
    BodyNode* body;
};
#endif
