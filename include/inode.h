#ifndef INTERPRETABLENODE_H
#define INTERPRETABLENODE_H

#include "node.h"
#include "interpretable.h"
class InterpretableNode : public Node, public Interpretable
{
 public:
	InterpretableNode(int type);
	virtual ~InterpretableNode();
	virtual Value interpret(Interpreter* interpreter) = 0;

};

#endif

