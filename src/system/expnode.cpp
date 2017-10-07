#include "expnode.h"
#include "variable.h"
#include "identifiernode.h"
#include "literalnode.h"
#include "statics.h"
#include "object.h"
#include "interpreter.h"
#include "validator.h"
#include <iostream>
#include <memory>
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
    else if(op == ">")
    {
        result = value1 > value2;
    }
    else if(op == ">=")
    {
        result = value1 >= value2;
    }
    else if(op == "<=")
    {
        result == value1 <= value2;
    }
    else if(op == "<")
    {
        result = value1 < value2;
    }
    else if(op == "==")
    {
        result = value1 == value2;
    }
    else if(op == "!=")
    {
        result = value1 != value2;
    }
    else if(op == "&&")
    {
        result = value1 && value2;
    }
    else if(op == "||")
    {
        result = value1 || value2;
    }
    else if(op == "=")
    {
        result = value2;
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

    FunctionSystem* old_fc_system;
    Scope* old_scope;
    Value left_v = this->left->interpret(interpreter);
    if (this->op == ".")
    {
        old_fc_system = interpreter->getFunctionSystem();
        old_scope = interpreter->getCurrentScope();
        // Left_v has the object to access
        std::shared_ptr<Object> obj = left_v.ovalue;
        Class* c = obj->getClass();
        if (left_v.holder != NULL)
        {
            // If the left variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (left_v.holder->name == "super")
                c = obj->getClass()->parent;
        }
        c->currentObj = obj;
        // The object scope is where all the attributes for the object ar stored so while we are accessing this object it should be set.
        interpreter->setCurrentScope(obj.get());
        interpreter->setFunctionSystem(c);
    }
    Value right_v = this->right->interpret(interpreter);
    if (this->op == ".")
    {
        // Restore the old function system
        interpreter->setFunctionSystem(old_fc_system);
        // Restore the old scope
        interpreter->setCurrentScope(old_scope);
        result = right_v;
        return result;
    }
    result = mathify(left_v, right_v, this->op);
    return result;
}

void ExpNode::test(Validator* validator)
{
    if (!validator->isExpecting())
        return;
    
    left->test(validator);
    right->test(validator);
}
