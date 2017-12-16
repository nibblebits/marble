#ifndef DOWHILENODE_H
#define DOWHILENODE_H
#include "inode.h"
#include "breakable.h"
class BodyNode;
class ExpressionInterpretableNode;
class DoWhileNode : public InterpretableNode, public Breakable
{
public:
    DoWhileNode();
    virtual ~DoWhileNode();
    virtual void didBreak(BREAK_TYPE type);
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ExpressionInterpretableNode* exp;
    BodyNode* body;
};
#endif
