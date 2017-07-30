#include "identifiernode.h"
#include "statics.h"

IdentifierNode::IdentifierNode() : Node(NODE_TYPE_IDENTIFIER)
{
	this->value = "";
}

IdentifierNode::~IdentifierNode()
{

}
