#include "vartype.h"
#include "variable.h"
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

VarType VarType::fromString(std::string value)
{
    VarType type;
    type.type = Variable::getVariableTypeForString(value);
    type.value = value;
    return type;
}
