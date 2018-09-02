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

void LimitScopeNode::test(Validator *validator, struct extras extra)
{
    try
    {
        /*
         * We need to create a scope for the limit_to
         */
        validator->new_parented_scope();
        Scope *limit_to_scope = validator->getCurrentScope();
        // Store the old scope for later
        Scope *old_previous_scope = limit_to_scope->prev;
        this->limit_to->test(validator, KEEP_SCOPE);

        // Now we don't want the scope to limit to have access to variables it shouldn't so set the limit_to_scope's previous scope to the validator root scope
        limit_to_scope->prev = validator->getRootScope();
        this->scope->test(validator);

        // Restore old previous scope and finish up
        limit_to_scope->prev = old_previous_scope;
        validator->finish_parented_scope();
    }
    catch (TestError &ex)
    {
        throw TestError(ex.getMessage() + " for limit scope statement");
    }
}

Value LimitScopeNode::interpret(Interpreter *interpreter, struct extras extra)
{
    /*
     * We must create a scope for the limit_to
     */
    interpreter->new_parented_scope();
    Scope *limit_to_scope = interpreter->getCurrentScope();
    Scope *old_previous_scope = limit_to_scope->prev;
    try
    {
        // Interpret the limit_to body so that variables to be shared with the scope can be created
        this->limit_to->interpret(interpreter, KEEP_SCOPE);
        // Now we don't want the scope to limit to have access to variables it shouldn't so set the limit_to_scope's previous scope to the interpreters root scope
        limit_to_scope->prev = interpreter->getRootScope();
        this->scope->interpret(interpreter);
        // Restore the old previous scope and finish up
        limit_to_scope->prev = old_previous_scope;
    }
    catch (...)
    {
        interpreter->finish_parented_scope();
        throw;
    }
    interpreter->finish_parented_scope();

    Value v;
    return v;
}

void LimitScopeNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    throw std::logic_error("Limit Scope Nodes cannot be evaluated");
}
