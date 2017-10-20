#include "function.h"
#include "interpreter.h"

Function::Function(FUNCTION_TYPE type, std::string name)
{
    this->type = type;
    this->name = name;
}

Function::~Function()
{
}

std::string Function::getName()
{
    return this->name;
}

