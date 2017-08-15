#include "node.h"
#include <iostream>
Node::Node(int type)
{
	this->type = type;
}

Node::~Node()
{
  std::cout << "Node deleted" << std::endl;
}

int Node::getType()
{
	return this->type;
}

void Node::setNext(Node* node)
{
	if (this->smart_next != NULL)
	{
		throw std::logic_error("This node already has a next node");
	}
	this->smart_next = std::unique_ptr<Node>(node);
	this->next = node;
}
