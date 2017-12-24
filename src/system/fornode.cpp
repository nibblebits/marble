#include "fornode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
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

void ForNode::test(Validator* validator)
{
    try
    {
        for (ExpressionInterpretableNode* node : init)
        {
    	    node->test(validator);
        }
        this->cond->test(validator);

        for (ExpressionInterpretableNode* node : loop)
        {
            node->test(validator);
        }
    }
    catch(TestError& ex)
    {
	    throw TestError(ex.getMessage() + " for \"for\" loop");
    }
}

Value ForNode::interpret(Interpreter* interpreter)
{
    // While nodes are breakable so lets tell the interpreter we are the current breakable
    interpreter->setCurrentBreakable(this);
    for (ExpressionInterpretableNode* node : init)
    {
        node->interpret(interpreter);
    }
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
        for (ExpressionInterpretableNode* node : loop)
        {
            node->interpret(interpreter);
        }
        v = cond->interpret(interpreter);
    }
    interpreter->finishBreakable();
    return v;
}

void ForNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of for nodes is not supported");
}
