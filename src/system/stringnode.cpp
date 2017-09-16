#include "stringnode.h"

StringNode::StringNode() : ExpressionInterpretableNode(NODE_TYPE_STRING)
{

}

StringNode::~StringNode()
{

}


Value StringNode::interpret(Interpreter* interpreter)
{
    Value result;
    result.type = VALUE_TYPE_STRING;
    result.svalue = this->value;
    result.activate();
    return result;
}

