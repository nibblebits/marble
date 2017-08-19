#include "varnode.h"

VarNode::VarNode() : Node(NODE_TYPE_VARIABLE_DECLARATION)
{
    this->type = NULL;
    this->name = NULL;
    this->value = NULL;
}

VarNode::~VarNode()
{

}
