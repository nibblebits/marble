#include "breaknode.h"
#include "interpreter.h"

BreakNode::BreakNode() : InterpretableNode(NODE_TYPE_BREAK)
{
}

BreakNode::~BreakNode()
{

}

void BreakNode::test(Validator* validator)
{
   // Nothing to test break is always valid.
}

Value BreakNode::interpret(Interpreter* interpreter)
{
    if (interpreter->hasBreakable())
    {
        interpreter->getCurrentBreakable()->breakNow(BREAK_TYPE_BREAK);
    }
 
    Value v;
    return v;   
}

void BreakNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Break nodes do not support evaluation");
}
