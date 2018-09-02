#include "identifiernode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "object.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "function.h"
#include "debug.h"
#include <iostream>

IdentifierNode::IdentifierNode() : ExpressionInterpretableNode(NODE_TYPE_IDENTIFIER)
{
    this->value = "";
}

IdentifierNode::~IdentifierNode()
{

}

bool IdentifierNode::objectHasConvertingMethod(SystemHandler* system_handler, VARIABLE_TYPE expecting_type, Variable* variable)
{
    /**
    * Let's check to see if this variable has a method that can return the correct value type
    * such as toString() or toNumber() in an object. When passed to a variable of type string toString() will be called on the object
    * the result will be returned. This applies for numbers also
    */
    if (variable->type == VARIABLE_TYPE_OBJECT)
    {
        Class *c = system_handler->getClassSystem()->getClassByName(variable->type_name);
        if (expecting_type == VARIABLE_TYPE_NUMBER 
                      || expecting_type == VARIABLE_TYPE_BOOLEAN)
        {
            if (c->hasFunction("toNumber"))
                return true;
        }
        else if(expecting_type == VARIABLE_TYPE_STRING)
        {
            if (c->hasFunction("toString"))
                return true;
        }
    }

    return false;
}

void IdentifierNode::test(Validator* validator, struct extras extra)
{
    ClassSystem* class_sys = validator->getClassSystem();
    // Let's first see if the variable is declared
    Scope* current_scope = validator->getCurrentScope();
    Variable* variable = current_scope->getVariableAnyScope(this->value);
    if (variable == NULL)
    {
        throw TestError("variable \"" + this->value + "\" is not declared");
    }

    // We don't really care about variable access unless the variable is in a class. Thats why we check if the object is not NULL
    if (variable->object != NULL && variable->access == MODIFIER_ACCESS_PRIVATE || variable->access == MODIFIER_ACCESS_PROTECTED)
    {
        Class* holder_class;
        std::shared_ptr<Object> current_obj = validator->getCurrentObject();
        if (!validator->isInClass())
        {
            /* If the validator is not in a class then this must mean its being accessed publicly from the global scope 
             * and not from within the class its self. So naturally the variable being private means this is illegal*/
           holder_class = variable->object->getClass();
           throw TestError("variable \"" + this->value + "\" is private or protected in class " + holder_class->name);
        }
        
        
        /*
         * Here we are doing this inside a class so we must check to see if we are allowed access.
         * we do this by ensuring that the variables class and our class match.
         * If they do not and the variable is private then this is illegal.
         *
         * Simularly for protected we check that either the current class is equal to the variable class or that the current class is an instance of the variable class
         * if it is not once again this is an error and its illegal.
        */
        Class* var_class = variable->object->getClass()->getClassWhoHasVariable(this->value);
        Class* current_class = validator->getCurrentClass();
        if (variable->access == MODIFIER_ACCESS_PRIVATE)
        {
            if (var_class != current_class)
            {
               throw TestError("variable \"" + this->value + "\" is private in class " + var_class->name);
            }
        }
        else
        {
            // This is a protected variable so lets check to see if this is valid
            if (var_class != current_class && !current_class->instanceOf(var_class))
            {
                throw TestError("variable \"" + this->value + "\" is protected in class " + var_class->name);
            }        
        }
        
   

    }
    
    if (!validator->isExpecting())
        return;
    
    // We now need to check if the type is valid
    VARIABLE_TYPE expecting_type = validator->getExpectingVariableType();

    if (variable->type != expecting_type)
    {
        // Object has a converting method so lets just return
        if (this->objectHasConvertingMethod(validator, expecting_type, variable))
            return;

        throw TestError("a " + variable->type_name + " " + (variable->isArray() ? " array" : "") + " was provided");
    }

    if (variable->type == VARIABLE_TYPE_OBJECT)
    {
        // We must ensure the object types are compatible
        if (!class_sys->isClassInstanceOf(variable->type_name, validator->getExpectingType()))
            throw TestError("a " + variable->type_name + " " + (variable->isArray() ? "array" : "") + " was provided");
    }
    
}

Value IdentifierNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    Variable* var = interpreter->getVariableByName(this->value);
    v = var->value;
    return v;
}

void IdentifierNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    Scope* current_scope = handler->getCurrentScope();
    if(expected_evaluation & EVALUATION_TYPE_DATATYPE)
    {
        if (expected_evaluation & EVALUATION_FROM_VARIABLE)
        {
            Variable* var = current_scope->getVariableAnyScope(this->value);
            if (var != NULL)
            {
 
                evaluation->type |= EVALUATION_TYPE_DATATYPE;
                evaluation->datatype.type = var->type;
                evaluation->datatype.value = var->type_name;
                evaluation->datatype.dimensions = var->dimensions;
                goto _next;
            }
        }

        evaluation->type |= EVALUATION_TYPE_DATATYPE;
        evaluation->datatype.type = Variable::getVariableTypeForString(this->value);
	    evaluation->datatype.value = this->value;
	    
    }
    
    _next:
    if (expected_evaluation & EVALUATION_TYPE_VARIABLE)
    {
        Variable* var = current_scope->getVariableAnyScope(this->value);
        if (var == NULL)
            throw std::logic_error("Variable not found for evaluation: " + this->value);
        evaluation->variable = var;
    }
    
}
