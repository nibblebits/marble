#include "expnode.h"

ExpNode::ExpNode() : ExpressionInterpretableNode(NODE_TYPE_EXPRESSION)
{
    this->left = NULL;
    this->right = NULL;
    this->op = "";
}

ExpNode::~ExpNode()
{

}




bool ExpNode::isAssignmentOperator()
{
    return this->op == "=" || this->op == "+=" || this->op == "-=" || this->op == "*=" || this->op == "/=";
}

Value ExpNode::interpret()
{
    return this->left->interpret() + this->right->interpret();
}
