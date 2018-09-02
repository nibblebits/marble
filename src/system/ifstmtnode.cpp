#include "ifstmtnode.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptions/testerror.h"
IfStatementNode::IfStatementNode() : Statement(NODE_TYPE_IF_STMT)
{
    this->exp = NULL;
    this->next = NULL;
}

IfStatementNode::~IfStatementNode()
{

}

void IfStatementNode::test(Validator* validator, struct extras extra)
{
    try
    {
    	//this->exp->test(validator);
   	    this->body->test(validator);
    }
    catch(TestError& ex)
    {
	    throw TestError(ex.getMessage() + " for if statement");
    }
}


Value IfStatementNode::interpret(Interpreter* interpreter, struct extras extra)
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
    else
    {
        // This is the next expression, it will be an else statement or an else if statement so lets interpret it
        if (this->next != NULL)
        {
            this->next->interpret(interpreter);
        }
    }   
    
    Value v;
    return v;
}

void IfStatementNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("If statements cannot be evaluated");
}
