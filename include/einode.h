#ifndef EXPRESSIONINTERPRETABLENODE_H
#define EXPRESSIONINTERPRETABLENODE_H

#include "inode.h"
class ExpressionInterpretableNode : public InterpretableNode
{
 public:
	ExpressionInterpretableNode(int type);
	virtual ~ExpressionInterpretableNode();
	virtual Value interpret(Interpreter* interpreter, struct extras extra) = 0;
	
};

#endif

