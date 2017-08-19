#ifndef EXPRESSIONINTERPRETABLENODE_H
#define EXPRESSIONINTERPRETABLENODE_H

#include "node.h"
#include "interpretable.h"
class ExpressionInterpretableNode : public Node, public Interpretable
{
 public:
	ExpressionInterpretableNode(int type);
	virtual ~ExpressionInterpretableNode();
	virtual Value interpret(Interpreter* interpreter) = 0;
};

#endif

