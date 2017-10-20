#include "singlefunction.h"

SingleFunction::SingleFunction(FUNCTION_TYPE type, std::string name, std::vector<VarType> argument_types) : Function(type, name)
{
    this->argument_types = argument_types;
}

SingleFunction::~SingleFunction()
{
}

