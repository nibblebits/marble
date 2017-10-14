#include "variable.h"
#include "object.h"
#include "scope.h"
Variable::Variable()
{
    this->name = "";
    this->type = -1;
    this->access = MODIFIER_ACCESS_PUBLIC;
    this->scope = NULL;
    this->object = NULL;
}

Variable::~Variable()
{
}

int Variable::getVariableTypeForString(std::string str)
{
    int type = VARIABLE_TYPE_OBJECT;
    if (str == "number")
    {
        type = VARIABLE_TYPE_NUMBER;
    }
    else if(str == "string")
    {
        type = VARIABLE_TYPE_STRING;
    }
    else if(str == "void")
    {
        type = VARIABLE_TYPE_VOID;
    }
    else if(str == "byte")
    {
        type = VARIABLE_TYPE_BYTE;
    }
    return type;
}


Variable Variable::getFromPointer(Variable* variable)
{
    Variable v;
    v.name = variable->name;
    v.type = variable->type;
    v.type_name = variable->type_name;
    v.access = variable->access;
    v.value.set(&variable->value);
    return v;
}


void Variable::set_value(Value value)
{
    this->value = value;
    this->value.holder = this;
}
