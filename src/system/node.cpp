#include "node.h"
#include <iostream>
Node::Node(int type) : type(type)
{
    this->next = NULL;
}

Node::~Node()
{

}

int Node::getType()
{
    return this->type;
}
