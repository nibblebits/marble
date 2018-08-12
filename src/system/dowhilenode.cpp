#include "dowhilenode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
DoWhileNode::DoWhileNode() : InterpretableNode(NODE_TYPE_DO_WHILE)
{

}

DoWhileNode::~DoWhileNode()
{

}

void DoWhileNode::didBreak(BREAK_TYPE type)
{
    this->body->breakNow(type);
}

void DoWhileNode::test(Validator* validator, struct extras extra)
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

Value DoWhileNode::interpret(Interpreter* interpreter, struct extras extra)
{
    // Do while nodes are breakable so lets tell the interpreter we are the current breakable
    interpreter->setCurrentBreakable(this);
    Value v;
    try
    {
        do
        {
            body->interpret(interpreter);
            v = exp->interpret(interpreter);
            if (isBroken())
            {
                BREAK_TYPE type = getBreakType();
                // Release the break
                releaseBreak();
                if (type == BREAK_TYPE_BREAK)
                    break;
                if (type == BREAK_TYPE_CONTINUE)
                    continue;
            }
        }
        while(v.dvalue == 1);
    } 
    catch(...)
    {
        interpreter->finishBreakable();
        throw;
    }
    
    interpreter->finishBreakable();
    return v;
}

void DoWhileNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of do while nodes is not supported");
}
