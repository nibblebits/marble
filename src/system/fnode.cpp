#include "fnode.h"
#include "nodes.h"
#include "statics.h"
#include "fnode.h"
#include "functionsystem.h"
#include "interpreter.h"
FunctionNode::FunctionNode() : ExpressionInterpretableNode(NODE_TYPE_FUNCTION)
{
    this->body = NULL;
    this->return_type = NULL;
}

FunctionNode::~FunctionNode()
{

}

Value FunctionNode::interpret(Interpreter* interpreter)
{
    interpreter->getFunctionSystem()->registerFunction(this);
    Value v;    
    return v;
}

