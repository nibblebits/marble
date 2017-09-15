#include "varnode.h"

VarNode::VarNode() : Node(NODE_TYPE_VARIABLE_DECLARATION)
{
    this->type = NULL;
    this->name = "";
    this->value = NULL;
    this->dimensions = 0;
}

VarNode::~VarNode()
{

}
