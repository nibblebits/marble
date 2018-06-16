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
    validator->save();
    this->index_node->test(validator);
    validator->restore();
    
    this->next_element->test(validator);
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
        return Value((double)c);
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
