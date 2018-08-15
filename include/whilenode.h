#ifndef WHILENODE_H
#define WHILENODE_H
#include "inode.h"
#include "breakable.h"
class BodyNode;
class ExpressionInterpretableNode;
class WhileNode : public InterpretableNode
{
public:
    WhileNode();
    virtual ~WhileNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ExpressionInterpretableNode* exp;
    BodyNode* body;
};
#endif
