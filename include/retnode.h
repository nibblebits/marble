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
    ExpressionInterpretableNode* exp; 
};
#endif
