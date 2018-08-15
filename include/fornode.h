#ifndef FORNODE_H
#define FORNODE_H
#include "inode.h"
class ListNode;
class BodyNode;
class ExpressionInterpretableNode;
class ForNode : public InterpretableNode
{
public:
    ForNode();
    virtual ~ForNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ListNode* init;
    ExpressionInterpretableNode* cond;
    ListNode* loop;
    BodyNode* body;
};
#endif
