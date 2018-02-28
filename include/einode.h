#ifndef EXPRESSIONINTERPRETABLENODE_H
#define EXPRESSIONINTERPRETABLENODE_H

#include "inode.h"
class ExpressionInterpretableNode : public InterpretableNode
{
 public:
	ExpressionInterpretableNode(int type);
	virtual ~ExpressionInterpretableNode();
	
};

#endif

