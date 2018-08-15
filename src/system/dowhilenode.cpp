#include "dowhilenode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include "exceptions/breakexception.h"
DoWhileNode::DoWhileNode() : InterpretableNode(NODE_TYPE_DO_WHILE)
{
}

DoWhileNode::~DoWhileNode()
{
}

void DoWhileNode::test(Validator *validator, struct extras extra)
{
    try
    {
        this->exp->test(validator);
        this->body->test(validator);
    }
    catch (TestError &ex)
    {
        throw TestError(ex.getMessage() + " for while statement");
    }
}

Value DoWhileNode::interpret(Interpreter *interpreter, struct extras extra)
{
    Value v;
    try
    {
        do
        {
            try
            {
                body->interpret(interpreter);
                v = exp->interpret(interpreter);
            }
            catch (BreakException &ex)
            {
                v = exp->interpret(interpreter);
                if (ex.type == BREAK_TYPE_BREAK)
                    break;
                if (ex.type == BREAK_TYPE_CONTINUE)
                    continue;
            }
        } while (v.dvalue == 1);
    }
    catch (...)
    {
        throw;
    }

    return v;
}

void DoWhileNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    throw std::logic_error("Evaluation of do while nodes is not supported");
}
