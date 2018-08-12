#include "arraynode.h"
#include "variable.h"
#include "array.h"
#include "validator.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <iostream>
ArrayNode::ArrayNode() : ExpressionInterpretableNode(NODE_TYPE_ARRAY)
{
    this->index_node = NULL;
    this->next_element = NULL;
}

ArrayNode::~ArrayNode()
{

}

void ArrayNode::test(Validator* validator, struct extras extra)
{
    extra.current_array_index+=1;
    validator->save();
    this->index_node->test(validator);
    validator->restore();

    if (this->next_element->type != NODE_TYPE_ARRAY)
    {
        // We have reached the end of the array access and we are now at the final element which will be the array variable
        struct Evaluation evaluation = this->next_element->evaluate(validator, EVALUATION_TYPE_DATATYPE | EVALUATION_FROM_VARIABLE);
        
        if (extra.current_array_index > evaluation.datatype.dimensions)
        {
            if (evaluation.datatype.type != VARIABLE_TYPE_STRING)
            {
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(validator->getClassSystem()->getClassByName("InvalidIndexException"))), "Attempting to access an invalid array dimension");
            }

            // We allow you to access characters of strings so one more index is allowed
            if (extra.current_array_index > evaluation.datatype.dimensions+1)
            {
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(validator->getClassSystem()->getClassByName("InvalidIndexException"))), "Attempting to access an invalid array dimension for string");
            }
        }

        this->next_element->test(validator, extra);
    }
    else
    {
        this->next_element->test(validator, extra);
    }
}

Value ArrayNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value index_exp = this->index_node->interpret(interpreter);
    
    // If this array node is being interpreted then the next_element is guaranteed to be an expression interpretable node.
    Value next_elem_value = this->next_element->interpret(interpreter);
    if (index_exp.dvalue > 0xffffffff || index_exp.dvalue < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidIndexException"))));
    }
    
    if (next_elem_value.type == VALUE_TYPE_STRING)
    {
        // This is a string lets get the character
        if (index_exp.dvalue >= next_elem_value.svalue.size())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidIndexException"))), "Index out of bounds for string");
        }

        char c = next_elem_value.svalue.at(index_exp.dvalue);
        return Value(std::to_string(c));
    }
    else
    {
        if(next_elem_value.avalue == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("NullPointerException"))));
        }
        else if(index_exp.dvalue >= next_elem_value.avalue->count)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidIndexException"))));
        }

        return next_elem_value.avalue->variables[(int)index_exp.dvalue].value;
    }
}

void ArrayNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    this->next_element->evaluate(handler, expected_evaluation, evaluation);
}
