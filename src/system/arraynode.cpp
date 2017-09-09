#include "arraynode.h"
ArrayNode::ArrayNode() : ExpressionInterpretableNode(NODE_TYPE_ARRAY)
{
    this->node = NULL;
    this->next_element = NULL;
}

ArrayNode::~ArrayNode()
{

}
    
Value ArrayNode::interpret(Interpreter* intrerpreter)
{

}
