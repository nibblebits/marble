#ifndef INTERPRETABLENODE_H
#define INTERPRETABLENODE_H

#include "evaluatingnode.h"
#include "interpretable.h"
class InterpretableNode : public EvaluatingNode, public Interpretable
{
 public:
	InterpretableNode(int type);
	virtual ~InterpretableNode();
	virtual Value interpret(Interpreter* interpreter) = 0;
	void ignoreValidation();
    bool shouldIgnoreValidation();
private:
	bool ignore_validation;
};

#endif

