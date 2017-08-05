#include "expnode.h"

ExpNode::ExpNode() : Node(NODE_TYPE_EXPRESSION)
{
	this->left = NULL;
	this->right = NULL;
	this->op = "";
}

ExpNode::~ExpNode()
{

}



