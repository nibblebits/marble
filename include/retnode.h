#ifndef RETURNNODE_H
#define RETURNNODE_H
#include "inode.h"
class ExpressionInterpretableNode;
class ReturnNode : public InterpretableNode
{
public:
    ReturnNode();
    virtual ~ReturnNode();
    virtual Value interpret(Interpreter* interpreter);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    ExpressionInterpretableNode* exp; 
};
#endif
