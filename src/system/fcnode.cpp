#include "fcnode.h"
#include "statics.h"
FunctionCallNode::FunctionCallNode() : Node(NODE_TYPE_FUNCTION_CALL)
{
    this->dest = NULL;
}

FunctionCallNode::~FunctionCallNode()
{

}
