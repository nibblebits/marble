#ifndef WHILENODE_H
#define WHILENODE_H
#include "inode.h"
#include "breakable.h"
class BodyNode;
class ExpressionInterpretableNode;
class WhileNode : public InterpretableNode, public Breakable
{
public:
    WhileNode();
    virtual ~WhileNode();
    virtual void didBreak(BREAK_TYPE type);
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ExpressionInterpretableNode* exp;
    BodyNode* body;
};
#endif