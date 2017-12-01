#include "whilenode.h"
#include "nodes.h"
#include "exceptions/testerror.h"
WhileNode::WhileNode() : InterpretableNode(NODE_TYPE_WHILE)
{

}

WhileNode::~WhileNode()
{

}
void WhileNode::test(Validator* validator)
{
    try
    {
    	this->exp->test(validator);
   	    this->body->test(validator);
    }
    catch(TestError& ex)
    {
	    throw TestError(ex.getMessage() + " for while statement");
    }
}

Value WhileNode::interpret(Interpreter* interpreter)
{
    Value v = exp->interpret(interpreter);
    while (v.dvalue == 1)
    {
        body->interpret(interpreter);
        v = exp->interpret(interpreter);
    }
    return v;
}

void WhileNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of while nodes is not supported");
}
