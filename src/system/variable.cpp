#include "variable.h"
#include <iostream>
Variable::Variable()
{
    this->name = "";
    this->type = -1;
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
