#include "node.h"

Node::Node(int type)
{
	this->type = type;
}

Node::~Node()
{

}

int Node::getType()
{
	return this->type;
}

