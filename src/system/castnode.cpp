#include "castnode.h"
#include "nodes.h"
#include "interpreter.h"
#include <iostream>
CastNode::CastNode() : ExpressionInterpretableNode(NODE_TYPE_CAST)
{
    this->casting_to = NULL;
    this->to_cast = NULL;
}

CastNode::~CastNode()
{

}

double CastNode::get_double_value(Value* from)
{
    switch(from->type)
    {
        case VALUE_TYPE_STRING:
        {
            return std::stod(from->svalue);
        }
        
        case VALUE_TYPE_NUMBER:
        {
            // Already number
            return from->dvalue;
        }
        
        default:
            throw std::logic_error("Unable to get double value from within CastNode");
    };
}

std::string CastNode::get_string_value(Value* from)
{
    switch(from->type)
    {
        case VALUE_TYPE_STRING:
        {
            // Already string
            return from->svalue;
        };
        
        case VALUE_TYPE_NUMBER:
        {
            return std::to_string(from->dvalue);
        };
        
        default:
            throw std::logic_error("Unable to get double value from within CastNode");
    };
}

Value CastNode::interpret(Interpreter* interpreter)
{
    Value v = this->to_cast->interpret(interpreter);
    struct Evaluation evaluation = this->casting_to->evaluate(interpreter, EVALUATION_TYPE_DATATYPE);
    if (evaluation.datatype.value == "number")
    {
        v.dvalue = (int) get_double_value(&v);
        v.type = VALUE_TYPE_NUMBER;
    }
    else if(evaluation.datatype.value == "string")
    {
        v.svalue = get_string_value(&v);
        v.type = VALUE_TYPE_STRING;
    }

    // Objects don't need to be casted this will work fine without any conversion.
    
    
    return v;
}

void CastNode::test(Validator* validator)
{    
    // Let's test what we are casting
    validator->save();
    this->to_cast->test(validator);
    validator->restore();
    
    struct Evaluation casting_to_evaluation = this->casting_to->evaluate(validator, EVALUATION_TYPE_DATATYPE);
    if (casting_to_evaluation.datatype.type == VARIABLE_TYPE_OBJECT)
    {
        // We must ensure this is legal casting of an object
        VALUE_TYPE expecting_type = validator->getExpectingType();
        if (expecting_type != VALUE_TYPE_OBJECT)
            throw std::logic_error("expecting a primitive type");
        
        
        // Ok lets check to see if this object cast is valid
        ClassSystem* class_sys = validator->getClassSystem();
        if (!class_sys->hasClassWithName(casting_to_evaluation.datatype.value))
            throw std::logic_error("the class with the name " + casting_to_evaluation.datatype.value + " has not been declared");
            
        struct Evaluation to_cast_evaluation = this->to_cast->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        if (to_cast_evaluation.datatype.type != VARIABLE_TYPE_OBJECT)
            throw std::logic_error("wanting to cast to " + casting_to_evaluation.datatype.value + " but you are attempting to cast from a primitive type");
        
        if (!class_sys->hasClassWithName(casting_to_evaluation.datatype.value))
            throw std::logic_error("the class with the name " + casting_to_evaluation.datatype.value + " has not been declared");
        std::cout << to_cast_evaluation.datatype.value << std::endl;
        Class* to_cast_class = class_sys->getClassByName(to_cast_evaluation.datatype.value);
        Class* casting_class = class_sys->getClassByName(casting_to_evaluation.datatype.value);
        if ((casting_class->name != to_cast_evaluation.datatype.value) && (!casting_class->instanceOf(to_cast_class)))
            throw std::logic_error("the class with the name " + casting_to_evaluation.datatype.value + " is not an instance of " + to_cast_evaluation.datatype.value);   
        
    }
    
    // All other types can be casted to eachother no need for a test.
}
    
