#include "fornode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include "exceptions/breakexception.h"
#include "listnode.h"
ForNode::ForNode() : InterpretableNode(NODE_TYPE_FOR)
{
}

ForNode::~ForNode()
{
}

void ForNode::test(Validator *validator, struct extras extra)
{
    // Let's create a new parented scope for this test
    validator->new_parented_scope();
    try
    {
        this->init->test(validator);
        this->cond->test(validator);
        this->loop->test(validator);
        this->body->test(validator);
    }
    catch (TestError &ex)
    {
        validator->finish_parented_scope();
        throw TestError(ex.getMessage() + " for \"for\" loop");
    }
    validator->finish_parented_scope();
}

Value ForNode::interpret(Interpreter *interpreter, struct extras extra)
{
    interpreter->new_parented_scope();
    init->interpret(interpreter);

    Value v = cond->interpret(interpreter);
    try
    {
        while (v.dvalue == 1)
        {
            try
            {
                body->interpret(interpreter);
                loop->interpret(interpreter);
                v = cond->interpret(interpreter);
            }
            catch (BreakException &ex)
            {
                loop->interpret(interpreter);
                v = cond->interpret(interpreter);
                if (ex.type == BREAK_TYPE_BREAK)
                    break;
                if (type == BREAK_TYPE_CONTINUE)
                    continue;
            }
        }
    }
    catch (...)
    {
        interpreter->finish_parented_scope();
        throw;
    }
    interpreter->finish_parented_scope();
    return v;
}

void ForNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    throw std::logic_error("Evaluation of for nodes is not supported");
}
