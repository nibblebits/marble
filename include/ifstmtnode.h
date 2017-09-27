#ifndef IFSTATEMENTNODE_H
#define IFSTATEMENTNODE_H
#include "statement.h"
#include "einode.h"
class IfStatementNode : public Statement
{
public:
    IfStatementNode();
    virtual ~IfStatementNode();
    virtual Value interpret(Interpreter* interpreter);
    ExpressionInterpretableNode* exp; 
};
#endif
