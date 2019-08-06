#include "referencevariable.h"
#include <exception>

ReferenceVariable::ReferenceVariable(Variable *variable_to_ref, int ref_index) : Variable()
{
    this->ref_index = ref_index;
    this->variable_to_ref = variable_to_ref;
}

ReferenceVariable::~ReferenceVariable()
{
}

bool ReferenceVariable::isReferencingIndex()
{
    return this->ref_index != -1;
}

void ReferenceVariable::setValue(Value value, Interpreter *interpreter)
{
    this->value = value;
    this->value.holder = variable_to_ref;

    switch (value.type)
    {
    case VALUE_TYPE_STRING:
        if (this->isReferencingIndex())
        {
            // Yeah we are setting this string by index however as their is no char type in Marble yet we just take the first index of a string provided to us. Could use a bit of improvement right?
            this->variable_to_ref->value.svalue[this->ref_index] = value.svalue[0];
        }
        else
        {
            this->variable_to_ref->value.svalue = value.svalue;
        }

        break;

    default:
        throw std::logic_error("not implemented");
    }
}