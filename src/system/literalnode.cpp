#include "literalnode.h"
#include "statics.h"
#include <iostream>
LiteralNode::LiteralNode() : ExpressionInterpretableNode(NODE_TYPE_LITERAL)
{

}

LiteralNode::~LiteralNode()
{

}

Value LiteralNode::interpret(Interpreter* interpreter)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = this->value;
    return v;
}
