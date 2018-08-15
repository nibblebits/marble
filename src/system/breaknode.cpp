#include "breaknode.h"
#include "interpreter.h"
#include "exceptions/breakexception.h"

BreakNode::BreakNode() : InterpretableNode(NODE_TYPE_BREAK)
{
}

BreakNode::~BreakNode()
{

}

void BreakNode::test(Validator* validator, struct extras extra)
{
   // Nothing to test break is always valid.
}

Value BreakNode::interpret(Interpreter* interpreter, struct extras extra)
{
    throw BreakException(BREAK_TYPE_BREAK); 
}

void BreakNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Break nodes do not support evaluation");
}
