#include "limitscopenode.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptions/testerror.h"
#include "bodynode.h"
LimitScopeNode::LimitScopeNode() : Statement(NODE_TYPE_LIMIT_SCOPE)
{
    this->limit_to = NULL;
    this->scope = NULL;
}

LimitScopeNode::~LimitScopeNode()
{

}

void LimitScopeNode::test(Validator* validator, struct extras extra)
{
    try
    {
        /*
         * We need to create a scope for the limit_to
         */
        validator->new_parented_scope();
        Scope* limit_to_scope = validator->getCurrentScope();
        Scope* old_previous_scope = limit_to_scope->prev;
    	this->limit_to->test(validator, KEEP_SCOPE);
        limit_to_scope->prev = validator->getRootScope();
   	    this->scope->test(validator);

        limit_to_scope->prev = old_previous_scope;
        validator->finish_parented_scope();
    }
    catch(TestError& ex)
    {
	    throw TestError(ex.getMessage() + " for limit scope statement");
    }
}


Value LimitScopeNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    return v;
}

void LimitScopeNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Limit Scope Nodes cannot be evaluated");
}
