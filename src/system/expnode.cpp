#include "expnode.h"
#include "variable.h"
#include "identifiernode.h"
#include "literalnode.h"
#include "statics.h"
#include "object.h"
#include "interpreter.h"
#include "validator.h"
#include "exceptionobject.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "exceptions/evaluationexception.h"
#include "debug.h"
#include "object.h"
#include "operator.h"
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
        Value v;
        v.set((double) (value1 == value2));
        result = v;
    }
    else if(op == "!=")
    {
        Value v;
        v.set((double) (value1 != value2));
        result = v;
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
Value ExpNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value result;
    if (isAssignmentOperator())
    {
        // This is to be an assignment. We must interpret the left node and then we will have the variable we need to assign to the right node
        Value left_v = left->interpret(interpreter);
        Variable* var_to_assign = left_v.holder;
        if (var_to_assign == NULL)
            throw std::logic_error("Problem as the value returned when interpreting the left node of this assignment does not have a holder: Value string: " + left_v.svalue);
        
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

        if (var_to_assign->is_locked)
        {
            // The variable we want to assign is locked so we just have to throw an exception and refuse this
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))), "The variable " + var_to_assign->name + " is locked and cannot be modified");
        }
        var_to_assign->setValue(result);
        return result;        
    }

    Value left_v = this->left->interpret(interpreter);

    /* Logical operators have rules for when we should continue or not */
    if (this->op == "&&" || this->op == "||")
    {
        if (this->op == "||")
        {
            // If our left expression is true then we can just return left_v here as the expression is true.
            if (left_v.dvalue)
                return left_v;
        }
        else
        {
            // If the left expression is false then we should not continue to the next expression as this is an AND operator
            if (!left_v.dvalue)
                return left_v;
        }
    }
    if (this->op == ".")
    {
        // Left_v has the object to access
        std::shared_ptr<Object> obj = left_v.ovalue;
        if (obj == NULL)
        {
            // Throw a NULL pointer exception here. This will be a SystemException so it may be caught from within marble.
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("NullPointerException"), {})));
        }
        
        Class* c = NULL;
        if (left_v.holder != NULL)
        {
            // If the left variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (left_v.holder->name == "super")
                c = obj->getClass()->parent;
        }
        
        Scope* accessors_scope = (extra.accessors_scope != NULL ? extra.accessors_scope : interpreter->getCurrentScope());
        // Interpret the right node on the object scope and return the result.
        obj->runThis([&] {
            struct extras extra;
            extra.accessors_scope = accessors_scope;
            extra.is_object_exp = true;
            extra.current_object = obj;
            result = this->right->interpret(interpreter, extra);
        }, interpreter, c);
        return result;
    }
    
    Value right_v = this->right->interpret(interpreter);
    result = mathify(left_v, right_v, this->op);
    return result;
}

void ExpNode::test(Validator* validator, struct extras extra)
{  
   if (this->op == ".")
   {
      test_obj_access(validator, extra);
   }
   else if(this->isAssignmentOperator())
   {
      test_assign(validator);
   }
   else
   {
      test_regular_exp(validator);
   }
    
}


void ExpNode::test_obj_access(Validator* validator, struct extras extra)
{
    /* While testing the left node we don't really want to care about testing weather or not a type matches
     * as we only care about this for the right operand. For example if an assignment is expecting a string it does not make sense
     * to check for the string on the left operand. "obj.var" The left operand is "obj" it would basically be saying I expect the object to be a string for the expression: 
     * "string str = obj.var;" This clearly does not make sense what we really want is to expect that the variable "var" of the object "obj" is a string.*/

    // Save the validators state and give us a blank state with no requirements. See comment above.
    validator->save();
    left->test(validator, extra);
    validator->restore();
    
    struct Evaluation evaluation;
    evaluation.extra = extra;
    left->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_TYPE_VARIABLE | EVALUATION_FROM_VARIABLE, &evaluation);

    Class* c = NULL;
    std::string class_name = evaluation.datatype.isArray() ? "array" : evaluation.datatype.value;
    if (!validator->isClassIgnored(class_name))
    {
        std::shared_ptr<Object> obj = validator->getClassObject(class_name);
        if (obj == NULL)
            throw std::logic_error("NULL object from validator: " + class_name);

        c = obj->getClass();
        if (evaluation.variable != NULL)
        {
            // If the evaluated variables name is "super" then we must be accessing a super-class and we should take the parent's class.
            if (evaluation.variable->name == "super")
                c = obj->getClass()->parent;
        }

        Scope* accessors_scope = (extra.accessors_scope != NULL ? extra.accessors_scope : validator->getCurrentScope());
        obj->runThis([&] {
            struct extras extra;
            extra.accessors_scope = accessors_scope;
            extra.is_object_exp = true;
            extra.current_object = obj;
            this->right->test(validator, extra);
        }, validator, c);   
    }
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
       throw TestError(std::string(ex.what()) + " but this assignment requires a " + evaluation.datatype.value + (evaluation.datatype.isArray() ? " with " + std::to_string(evaluation.datatype.dimensions) + " array dimensions" : ""));
    }
    validator->endExpecting();
}

void ExpNode::test_regular_exp(Validator* validator)
{
    if (Operator::isCompareOperator(this->op))
    {
        /*
         * We are a compare operator so let's ensure that the validator is expecting a boolean
         */
        if(validator->isExpecting() && validator->getExpectingType() != "boolean")
        {
            throw TestError("a boolean was provided");
        } 

        // Good so far so good now we need to save the validators state as we don't want to apply these rules to the left and right operands
        validator->save();
    }

    // Test the left and right nodes
    left->test(validator);
    right->test(validator);

    if (Operator::isCompareOperator(this->op))
    {
        // Let's restore the validator
        validator->restore();
    }
}

void ExpNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    if (expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        VarType left_vartype;
        left->evaluate(handler, expected_evaluation, evaluation);
        left_vartype = evaluation->datatype;
        FunctionSystem* old_fc_system;
        Scope* old_scope;
        if (this->op == ".")
        {
            if (handler->getType() != SYSTEM_HANDLER_VALIDATOR)
                throw std::logic_error("Evaluating the access of classes or objects is only allowed for validators"); 
            
            old_fc_system = handler->getFunctionSystem();
            old_scope = handler->getCurrentScope();   
                 
            Validator* validator = (Validator*) handler;
            std::shared_ptr<Object> obj = validator->getClassObject(evaluation->datatype.value);
            handler->setCurrentScope(obj.get());
            handler->setFunctionSystem(obj->getClass());
        }

        evaluation->extra.is_object_exp = true;
        right->evaluate(handler, expected_evaluation, evaluation);
        if (this->op != ".")
        {
            if (left_vartype != evaluation->datatype)
            {
                throw EvaluationException("Evaluation error as both expression nodes differ in types", left_vartype.value, evaluation->datatype.value);
            }
        }

        if (this->op == ".")
        {
            handler->setCurrentScope(old_scope);
            handler->setFunctionSystem(old_fc_system);
        }
    }
}
