#include "expnode.h"
#include "variable.h"
#include "identifiernode.h"
#include "literalnode.h"
#include "statics.h"
#include "object.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptions/testerror.h"
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

/*
* this must be split into multiple methods in the same way the test routine is.
*/
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
        var_to_assign->set_value(result);
        return result;        
    }

    Value left_v = this->left->interpret(interpreter);
    if (this->op == ".")
    {
        // Left_v has the object to access
        std::shared_ptr<Object> obj = left_v.ovalue;
        Class* c = NULL;
        if (left_v.holder != NULL)
        {
            // If the left variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (left_v.holder->name == "super")
                c = obj->getClass()->parent;
        }
        
        // Interpret the right node on the object scope and return the result.
        obj->runThis([&] {
            result = this->right->interpret(interpreter);
        }, c);
        return result;
    }
    
    Value right_v = this->right->interpret(interpreter);
    result = mathify(left_v, right_v, this->op);
    return result;
}

void ExpNode::test(Validator* validator)
{    
   if (this->op == ".")
   {
      test_obj_access(validator);
   }
   else if(this->op == "=")
   {
      test_assign(validator);
   }
   else
   {
      test_regular_exp(validator);
   }
    
}


void ExpNode::test_obj_access(Validator* validator)
{
    left->test(validator);
    struct Evaluation evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_TYPE_VARIABLE | EVALUATION_FROM_VARIABLE);
    
    if (this->op == ".")
    {
        Class* c = NULL;
        Object* obj = validator->getClassObject(evaluation.datatype.value);
        if (obj == NULL)
            throw std::logic_error("NULL object from validator: " + evaluation.datatype.value);
            
        if (evaluation.variable != NULL)
        {
            // If the evaluated variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (evaluation.variable->name == "super")
                c = obj->getClass()->parent;
        }
 
        obj->runThis([&] {
            this->right->test(validator);
        }, c);
        return;
    }
    
    right->test(validator);
    
}

void ExpNode::test_assign(Validator* validator)
{
    left->test(validator);
    struct Evaluation evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    validator->expecting(evaluation.datatype.value);
    
    try
    {
        right->test(validator);
    } catch(TestError& ex)
    {
       throw TestError(std::string(ex.what()) + " but this assignment requires a " + evaluation.datatype.value);
    }
    validator->endExpecting();
}

void ExpNode::test_regular_exp(Validator* validator)
{
    left->test(validator);
    right->test(validator);
}

void ExpNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        left->evaluate(handler, expected_evaluation, evaluation);
        FunctionSystem* old_fc_system;
        Scope* old_scope;
        if (this->op == ".")
        {
            if (handler->getType() != SYSTEM_HANDLER_VALIDATOR)
                throw std::logic_error("Evaluating the access of classes or objects is only allowed for validators"); 
            
            old_fc_system = handler->getFunctionSystem();
            old_scope = handler->getCurrentScope();   
                 
            Validator* validator = (Validator*) handler;
            Object* obj = validator->getClassObject(evaluation->datatype.value);
            handler->setCurrentScope(obj);
            handler->setFunctionSystem(obj->getClass());
        }
        right->evaluate(handler, expected_evaluation, evaluation);
        
        if (this->op == ".")
        {
            handler->setCurrentScope(old_scope);
            handler->setFunctionSystem(old_fc_system);
        }
    }
}
