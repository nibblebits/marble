#include "outputnode.h"
#include "interpreter.h"
#include "validator.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"

OutputNode::OutputNode() : ExpressionInterpretableNode(NODE_TYPE_OUTPUT_NODE)
{

}

OutputNode::~OutputNode()
{

}

void OutputNode::test(Validator* validator, struct extras extra)
{
    this->exp->test(validator);
}

Value OutputNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v = this->exp->interpret(interpreter, extra);
    if (v.type != VALUE_TYPE_STRING)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidTypeException"), {})), "Direct output is only allowed for strings. You should cast this", interpreter->getStackTraceLog());
    }

    interpreter->output(v.svalue.c_str(), v.svalue.size());
    return v;
}   

void OutputNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{

}