#include "thrownode.h"
#include "einode.h"
#include "validator.h"
#include "exceptions/systemexception.h"
#include "exceptions/testerror.h"
ThrowNode::ThrowNode() : InterpretableNode(NODE_TYPE_THROW)
{
    this->exp = NULL;
}

ThrowNode::~ThrowNode()
{

}

void ThrowNode::test(Validator* validator)
{
    try
    {
        validator->expecting("Exception");
        this->exp->test(validator);
        validator->endExpecting();
    }
    catch(TestError& ex)
    {
        throw TestError(ex.getMessage() + " while throwing");
    }
}

Value ThrowNode::interpret(Interpreter* interpreter)
{
    Value v = this->exp->interpret(interpreter);
    // Lets throw this system exception
    throw SystemException(v.ovalue);
    return v;
}

void ThrowNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluation of throw nodes is not supported");
}
