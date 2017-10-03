#include "function.h"
#include "interpreter.h"

Function::Function(std::string name)
{
    this->name = name;
}

Function::~Function()
{
}

std::string Function::getName()
{
    return this->name;
}

