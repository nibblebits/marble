#include "continuenode.h"
#include "interpreter.h"

ContinueNode::ContinueNode() : InterpretableNode(NODE_TYPE_CONTINUE)
{
}

ContinueNode::~ContinueNode()
{

}

void ContinueNode::test(Validator* validator, struct extras extra)
{
   // Nothing to test break is always valid.
}

Value ContinueNode::interpret(Interpreter* interpreter, struct extras extra)
{
    if (interpreter->hasBreakable())
    {
        interpreter->getCurrentBreakable()->breakNow(BREAK_TYPE_CONTINUE);
    }
 
    Value v;
    return v;   
}

void ContinueNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Break nodes do not support evaluation");
}
