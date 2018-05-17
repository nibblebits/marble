#ifndef FORNODE_H
#define FORNODE_H
#include "inode.h"
#include "breakable.h"
class ListNode;
class BodyNode;
class ExpressionInterpretableNode;
class ForNode : public InterpretableNode, public Breakable
{
public:
    ForNode();
    virtual ~ForNode();
    virtual void didBreak(BREAK_TYPE type);
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ListNode* init;
    ExpressionInterpretableNode* cond;
    ListNode* loop;
    BodyNode* body;
};
#endif
