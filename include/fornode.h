#ifndef FORNODE_H
#define FORNODE_H
#include "inode.h"
#include "breakable.h"
class BodyNode;
class ExpressionInterpretableNode;
class ForNode : public InterpretableNode, public Breakable
{
public:
    ForNode();
    virtual ~ForNode();
    virtual void didBreak(BREAK_TYPE type);
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    std::vector<ExpressionInterpretableNode*> init;
    ExpressionInterpretableNode* cond;
    std::vector<ExpressionInterpretableNode*> loop;
    BodyNode* body;
};
#endif
