#include "fornode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include "listnode.h"
ForNode::ForNode() : InterpretableNode(NODE_TYPE_FOR)
{

}

ForNode::~ForNode()
{

}

void ForNode::didBreak(BREAK_TYPE type)
{
    this->body->breakNow(type);
}

void ForNode::test(Validator* validator, struct extras extra)
{
    // Let's create a new parented scope for this test
    validator->new_parented_scope();
    try
    {
    	this->init->test(validator);
        this->cond->test(validator);
        this->loop->test(validator);
        this->body->test(validator);
    }
    catch(TestError& ex)
    {
        validator->finish_parented_scope();
	    throw TestError(ex.getMessage() + " for \"for\" loop");
    }
    validator->finish_parented_scope();
}

Value ForNode::interpret(Interpreter* interpreter, struct extras extra)
{
    interpreter->new_parented_scope();
    // For nodes are breakable so lets tell the interpreter we are the current breakable
    interpreter->setCurrentBreakable(this);
    init->interpret(interpreter);

    Value v = cond->interpret(interpreter);
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
        loop->interpret(interpreter);
        v = cond->interpret(interpreter);
    }
    interpreter->finishBreakable();
    interpreter->finish_parented_scope();
    return v;
}

void ForNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of for nodes is not supported");
}
