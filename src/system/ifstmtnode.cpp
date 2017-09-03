#include "ifstmtnode.h"
#include "interpreter.h"
IfStatementNode::IfStatementNode() : Statement(NODE_TYPE_IF_STMT)
{

}

IfStatementNode::~IfStatementNode()
{

}


void IfStatementNode::interpret(Interpreter* interpreter)
{
    // Test the expression
    Value result = exp->interpret(interpreter);
    // Do we have a true result?
    if (result.dvalue == 1)
    {
        // We do lets execute
        interpreter->interpret_body(body);
    }   
}
