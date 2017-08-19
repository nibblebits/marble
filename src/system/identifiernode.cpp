#include "identifiernode.h"
#include "statics.h"
#include "interpreter.h"
#include <iostream>

IdentifierNode::IdentifierNode() : ExpressionInterpretableNode(NODE_TYPE_IDENTIFIER)
{
    this->value = "";
}

IdentifierNode::~IdentifierNode()
{

}

Value IdentifierNode::interpret(Interpreter* interpreter)
{
    Value v = interpreter->getVariableByName(this->value)->value;
    return v;
}
