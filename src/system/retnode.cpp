#include "retnode.h"
#include "einode.h"
ReturnNode::ReturnNode() : InterpretableNode(NODE_TYPE_RETURN)
{
    this->exp = NULL;
}

ReturnNode::~ReturnNode()
{

}

Value ReturnNode::interpret(Interpreter* interpreter)
{
    Value v;
    if (this->exp != NULL)
        v = this->exp->interpret(interpreter);
    return v;
}
