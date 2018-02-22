#ifndef IFSTATEMENTNODE_H
#define IFSTATEMENTNODE_H
#include "statement.h"
#include "einode.h"
class IfStatementNode : public Statement
{
public:
    IfStatementNode();
    virtual ~IfStatementNode();
    virtual void test(Validator* validator, struct extras extra);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ExpressionInterpretableNode* exp; 
    ExpressionInterpretableNode* next;
};
#endif
