#include "castnode.h"
#include "nodes.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/testerror.h"
#include "exceptions/systemexception.h"
#include "object.h"
#include "value.h"
#include <iostream>
CastNode::CastNode() : ExpressionInterpretableNode(NODE_TYPE_CAST)
{
    this->casting_to = NULL;
    this->to_cast = NULL;
}

CastNode::~CastNode()
{

}

Value CastNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v = this->to_cast->interpret(interpreter);
    struct Evaluation evaluation = this->casting_to->evaluate(interpreter, EVALUATION_TYPE_DATATYPE);
    try
    {
        if (evaluation.datatype.value == "number")
        {
            v.dvalue = (double) Value::getDoubleValue(&v);
            v.type = VALUE_TYPE_NUMBER;
            v.type_str = "number";
        }
        else if(evaluation.datatype.value == "int")
        {
            v.dvalue = (int) Value::getDoubleValue(&v);
            v.type = VALUE_TYPE_NUMBER;
            v.type_str = "number";
        }
        else if(evaluation.datatype.value == "boolean")
        {
            // Let's just change the type string to boolean. As all booleans hold numbers nothing else needs to be changed
            v.type_str = "boolean";
        }
        else if(evaluation.datatype.value == "string")
        {
            v.svalue = Value::getStringValue(&v);
            v.type = VALUE_TYPE_STRING;
            v.type_str = "string";
        }
        else
        {
            if (evaluation.datatype.type == VARIABLE_TYPE_OBJECT)
            {
                // For casting of objects just change the type string for the value.
                v.type_str = evaluation.datatype.value;
            }
        }
    }
    catch(...)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidCastException"), {})));
    }


    // Nothing to do for all other situations

    return v;
}

void CastNode::test(Validator* validator, struct extras extra)
{    
    // Let's test what we are casting
    validator->save();
    this->to_cast->test(validator);
    validator->restore();
    
    struct Evaluation casting_to_evaluation = this->casting_to->evaluate(validator, EVALUATION_TYPE_DATATYPE);
    if (casting_to_evaluation.datatype.type == VARIABLE_TYPE_OBJECT)
    {
        // We must ensure this is legal casting of an object
        VALUE_TYPE expecting_type = validator->getExpectingValueType();
  
        // Ok lets check to see if this object cast is valid
        ClassSystem* class_sys = validator->getClassSystem();
        if (!class_sys->hasClassWithName(casting_to_evaluation.datatype.value))
            throw TestError("the class with the name " + casting_to_evaluation.datatype.value + " has not been declared");
            
        struct Evaluation to_cast_evaluation = this->to_cast->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        if (to_cast_evaluation.datatype.type != VARIABLE_TYPE_OBJECT)
            throw TestError("wanting to cast to " + casting_to_evaluation.datatype.value + " but you are attempting to cast from a primitive type");
        
        if (!class_sys->hasClassWithName(casting_to_evaluation.datatype.value))
            throw TestError("the class with the name " + casting_to_evaluation.datatype.value + " has not been declared");
        Class* to_cast_class = class_sys->getClassByName(to_cast_evaluation.datatype.value);
        Class* casting_class = class_sys->getClassByName(casting_to_evaluation.datatype.value); 
        if ((casting_class->name != to_cast_evaluation.datatype.value) && (!casting_class->instanceOf(to_cast_class) && !to_cast_class->instanceOf(casting_class)))
            throw TestError("the class with the name " + casting_to_evaluation.datatype.value + " is not an instance of " + to_cast_evaluation.datatype.value);   
        
    }
    
    // All other types can be casted to eachother no need for a test.
}

void CastNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    this->casting_to->evaluate(handler, expected_evaluation, evaluation);
}
