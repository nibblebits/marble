#include "identifiernode.h"
#include "statics.h"
#include "interpreter.h"
#include "object.h"
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
    Variable* var = interpreter->getVariableByName(this->value);
    Value v = var->value;
    return v;
}
