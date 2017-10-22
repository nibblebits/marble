#include "literalnode.h"
#include "statics.h"
#include "validator.h"
#include "interpreter.h"
#include "exceptions/testerror.h"
#include <iostream>
LiteralNode::LiteralNode() : ExpressionInterpretableNode(NODE_TYPE_LITERAL)
{

}

LiteralNode::~LiteralNode()
{

}

void LiteralNode::test(Validator* validator)
{
    if (!validator->isExpecting())
        return;
    VALUE_TYPE expecting_type = validator->getExpectingValueType();
    if (expecting_type != VALUE_TYPE_NUMBER)
    {
        throw TestError("a number was provided");
    }
}
Value LiteralNode::interpret(Interpreter* interpreter)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = this->value;
    return v;
}

void LiteralNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        // Since this is a literal node and they are expecting a datatype we should return a "number" type as this is the standard type for literal nodes.
        evaluation->datatype.type = VARIABLE_TYPE_NUMBER;
        evaluation->datatype.value = "number"; 
    }
}
