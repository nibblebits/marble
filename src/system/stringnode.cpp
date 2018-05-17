#include "stringnode.h"
#include "validator.h"
#include "exceptions/testerror.h"

StringNode::StringNode() : ExpressionInterpretableNode(NODE_TYPE_STRING)
{

}

StringNode::~StringNode()
{

}


void StringNode::test(Validator* validator, struct extras extra)
{
    if (!validator->isExpecting())
        return;
    VALUE_TYPE expecting_type = validator->getExpectingValueType();
    if (expecting_type != VALUE_TYPE_STRING)
    {
        throw TestError("a string was provided");
    }

}

Value StringNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value result;
    result.type = VALUE_TYPE_STRING;
    result.svalue = this->value;
    return result;
}

void StringNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        // Strings default to the STRING datatype.
        evaluation->datatype.type = VARIABLE_TYPE_STRING;
        evaluation->datatype.value = "string";
    }
}
