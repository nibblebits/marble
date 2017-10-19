#include "stringnode.h"
#include "validator.h"

StringNode::StringNode() : ExpressionInterpretableNode(NODE_TYPE_STRING)
{

}

StringNode::~StringNode()
{

}


void StringNode::test(Validator* validator)
{
    if (!validator->isExpecting())
        return;
    VALUE_TYPE expecting_type = validator->getExpectingValueType();
    if (expecting_type != VALUE_TYPE_STRING)
    {
        throw std::logic_error("a string was provided");
    }

}

Value StringNode::interpret(Interpreter* interpreter)
{
    Value result;
    result.type = VALUE_TYPE_STRING;
    result.svalue = this->value;
    return result;
}

