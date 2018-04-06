#include "vartype.h"
#include "variable.h"
#include "csystem.h"
#include "class.h"
#include <iostream>
VarType::VarType()
{
    this->type = -1;
    this->value = "";
}

VarType::~VarType()
{

}

bool VarType::operator==(const VarType &other) const
{
    if (this->type != other.type)
        return false;
    if (this->value != other.value)
        return false;
    
    return true;
}

bool VarType::operator!=(const VarType &other) const
{
    if (this->type == other.type && this->value == other.value)
        return false;
    return true;
}


bool VarType::ensureCompatibility(const VarType& other, ClassSystem* c_system)
{   
    if (this->type == VARIABLE_TYPE_OBJECT && other.type == VARIABLE_TYPE_OBJECT)
    {
        Class* f1_type_cls = c_system->getClassByName(this->value);
        Class* f2_type_cls = c_system->getClassByName(other.value);
        if (f1_type_cls == NULL || f2_type_cls == NULL)
            return false;

        if (!f1_type_cls->instanceOf(f2_type_cls))
        {
            return false;
        }
        return true;
    }
    else
    {
        return this->type == other.type;
    }
}
VarType VarType::fromString(std::string value)
{
    VarType type;
    type.type = Variable::getVariableTypeForString(value);
    type.value = value;
    return type;
}
