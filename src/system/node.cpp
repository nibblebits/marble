#include "node.h"
#include <iostream>
Node::Node(int type) : type(type)
{
}

Node::~Node()
{

}

int Node::getType()
{
    return this->type;
}

