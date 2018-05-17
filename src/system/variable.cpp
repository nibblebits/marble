#include "variable.h"
#include "object.h"
#include "array.h"
#include "scope.h"
Variable::Variable()
{
    this->name = "";
    this->type = -1;
    this->dimensions = 0;
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
    else if(str == "int")
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
    else if (str == "boolean")
    {
        type = VARIABLE_TYPE_BOOLEAN;
    }
    return type;
}


Variable Variable::getFromPointer(Variable* variable)
{
    Variable v;
    v.name = variable->name;
    v.type = variable->type;
    v.dimensions = variable->dimensions;
    v.type_name = variable->type_name;
    v.access = variable->access;
    v.value.set(&variable->value);
    return v;
}

bool Variable::isArray()
{
    return this->dimensions > 0;
}

void Variable::setValue(Value value)
{
    this->value = value;
    this->value.holder = this;
}

void Variable::setValue(std::string value)
{
    Value v;
    v.type = VALUE_TYPE_STRING;
    v.svalue = value;
    setValue(v);
}

void Variable::setValue(double value)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = value;
    setValue(v);
}

void Variable::setValue(std::shared_ptr<Object> value)
{
    bool is_array = std::dynamic_pointer_cast<Array>(value) != NULL;
    if (is_array)
        throw std::logic_error("Arrays cannot currently be set using these helper methods as this functionality is not implemented");

    Value v;
    v.type = VALUE_TYPE_OBJECT;
    v.ovalue = value;
    setValue(v);
}