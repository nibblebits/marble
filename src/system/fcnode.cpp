#include "fcnode.h"
#include "statics.h"
#include "interpreter.h"
FunctionCallNode::FunctionCallNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION_CALL)
{
    this->dest = NULL;
}

FunctionCallNode::~FunctionCallNode()
{

}

Value FunctionCallNode::interpret(Interpreter* interpreter)
{
    Value value;
    return value;
}
