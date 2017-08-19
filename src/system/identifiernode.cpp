#include "identifiernode.h"
#include "statics.h"

IdentifierNode::IdentifierNode() : ExpressionInterpretableNode(NODE_TYPE_IDENTIFIER)
{
    this->value = "";
}

IdentifierNode::~IdentifierNode()
{

}

Value IdentifierNode::interpret()
{
    Value v;
// Remember we are not returning the identifier as a string but instead what the identifier is pointing to
    return v;
}
