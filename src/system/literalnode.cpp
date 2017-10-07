#include "literalnode.h"
#include "statics.h"
#include "validator.h"
#include "interpreter.h"
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
    VALUE_TYPE expecting_type = validator->getExpectingType();
    if (expecting_type != VALUE_TYPE_NUMBER)
    {
        throw std::logic_error("a number was provided");
    }
}
Value LiteralNode::interpret(Interpreter* interpreter)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = this->value;
    return v;
}
