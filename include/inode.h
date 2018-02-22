#ifndef INTERPRETABLENODE_H
#define INTERPRETABLENODE_H

#include "evaluatingnode.h"
#include "interpretable.h"
class Interpreter;
class InterpretableNode : public EvaluatingNode, public Interpretable
{
 public:
	InterpretableNode(int type);
	virtual ~InterpretableNode();
	void test(Validator* validator);
    Value interpret(Interpreter* interpreter);
    virtual Value interpret(Interpreter* interpreter, struct extras extra) = 0;
	void ignoreValidation();
    bool shouldIgnoreValidation();
private:
	bool ignore_validation;
};

#endif

