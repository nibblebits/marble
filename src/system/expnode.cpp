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

    FunctionSystem* old_fc_system;
    Scope* old_scope;
    std::shared_ptr<Object> old_obj;
    Class* c;
    Value left_v = this->left->interpret(interpreter);
    if (this->op == ".")
    {
        old_fc_system = interpreter->getFunctionSystem();
        old_scope = interpreter->getCurrentScope();
        // Left_v has the object to access
        std::shared_ptr<Object> obj = left_v.ovalue;
        c = obj->getClass();
        if (left_v.holder != NULL)
        {
            // If the left variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (left_v.holder->name == "super")
                c = obj->getClass()->parent;
        }
        old_obj = c->currentObj;
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
        // Restore the old class object.
        c->currentObj = old_obj;
        result = right_v;
        return result;
    }
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
    struct Evaluation evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    FunctionSystem* old_fc_system;
    Scope* old_scope;
    
    if (this->op == ".")
    {
        old_fc_system = validator->getFunctionSystem();
        old_scope = validator->getCurrentScope();   
                     
        Object* obj = validator->getClassObject(evaluation.datatype.value);
        validator->setCurrentScope(obj);
        validator->setFunctionSystem(obj->getClass());
    }
    
    right->test(validator);
    
    if (this->op == ".")
    {
        validator->setCurrentScope(old_scope);
        validator->setFunctionSystem(old_fc_system);
    }
}

void ExpNode::test_assign(Validator* validator)
{
    left->test(validator);
    struct Evaluation evaluation = left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
    if (evaluation.datatype.type == VARIABLE_TYPE_OBJECT)
    {
        validator->expectingObject(evaluation.datatype.value);
    }
    else
    {
        validator->expecting(evaluation.datatype.type);
    }
    try
    {
        right->test(validator);
    } catch(std::logic_error& ex)
    {
       throw std::logic_error(std::string(ex.what()) + " but this assignment requires a " + evaluation.datatype.value);
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
