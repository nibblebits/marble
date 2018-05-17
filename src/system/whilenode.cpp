#include "whilenode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
WhileNode::WhileNode() : InterpretableNode(NODE_TYPE_WHILE)
{

}

WhileNode::~WhileNode()
{

}

void WhileNode::didBreak(BREAK_TYPE type)
{
    this->body->breakNow(type);
}

void WhileNode::test(Validator* validator, struct extras extra)
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

Value WhileNode::interpret(Interpreter* interpreter, struct extras extra)
{
    // While nodes are breakable so lets tell the interpreter we are the current breakable
    interpreter->setCurrentBreakable(this);
    Value v = exp->interpret(interpreter);
    while (v.dvalue == 1)
    {
        body->interpret(interpreter);
        if (isBroken())
        {
            BREAK_TYPE type = getBreakType();
            if (type == BREAK_TYPE_BREAK)
                break;
            if (type == BREAK_TYPE_CONTINUE)
                continue;
        }
        v = exp->interpret(interpreter);
    }
    interpreter->finishBreakable();
    return v;
}

void WhileNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of while nodes is not supported");
}
