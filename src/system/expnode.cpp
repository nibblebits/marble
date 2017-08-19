#include "expnode.h"
#include "variable.h"
#include "identifiernode.h"
#include "literalnode.h"
#include "statics.h"

ExpNode::ExpNode() : ExpressionInterpretableNode(NODE_TYPE_EXPRESSION)
{
    this->left = NULL;
    this->right = NULL;
    this->op = "";
}

ExpNode::~ExpNode()
{

}




bool ExpNode::isAssignmentOperator()
{
    return this->op == "=" || this->op == "+=" || this->op == "-=" || this->op == "*=" || this->op == "/=";
}

Value ExpNode::mathify(Value& value1, Value& value2, std::string op)
{
    Value result;
    if (op == "+" || op == "+=")
    {
        result = value1 + value2;
    }
    else if(op == "-" || op == "-=")
    {
        result = value1 - value2;
    }
    else if(op == "*" || op == "*=")
    {
        result = value1 * value2;
    }
    else if(op == "/" || op == "/=")
    {
        result = value1 / value2;
    }
    else
    {
        throw std::logic_error("Value ExpNode::mathify(Value value1, Value2 value2, std::string op): Invalid operator provided");
    }

    return result;
}

Value ExpNode::interpret(Interpreter* interpreter)
{
    Value result;
    if (isAssignmentOperator())
    {
        // This is to be an assignment. We must interpret the left node and then we will have the variable we need to assign to the right node
        Value left_v = left->interpret(interpreter);
        Variable* var_to_assign = left_v.holder;

        // Ok now lets get the value and assign it to the variable
        Value right_v = right->interpret(interpreter);

        if (this->op == "=")
        {      
            result = right_v;
        }
        else
        {
            result = mathify(left_v, right_v, this->op);
        }
        var_to_assign->value = result;
        return result;        
    }


    Value left = this->left->interpret(interpreter);
    Value right = this->right->interpret(interpreter);
    result = mathify(left, right, this->op);
    return result;
}
