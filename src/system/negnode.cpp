#include "negnode.h"
#include "nodes.h"
#include <iostream>
NegNode::NegNode() : ExpressionInterpretableNode(NODE_TYPE_NEGATIVE)
{
    this->node = NULL;
}

NegNode::~NegNode()
{

}

Value NegNode::interpret(Interpreter* interpreter)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = -this->node->interpret(interpreter).dvalue;
    return v;

}
