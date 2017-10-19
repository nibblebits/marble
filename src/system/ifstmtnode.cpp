#include "ifstmtnode.h"
#include "interpreter.h"
IfStatementNode::IfStatementNode() : Statement(NODE_TYPE_IF_STMT)
{
    this->exp = NULL;
}

IfStatementNode::~IfStatementNode()
{

}


Value IfStatementNode::interpret(Interpreter* interpreter)
{
    if (exp == NULL)
        throw std::logic_error("Expecting an expression");
    
    // Test the expression
    Value result = exp->interpret(interpreter);
    // Do we have a true result?
    if (result.dvalue == 1)
    {
        // We do lets execute
        body->interpret(interpreter);
    }   
    
    Value v;
    return v;
}

void IfStatementNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("If statements cannot be evaluated");
}
