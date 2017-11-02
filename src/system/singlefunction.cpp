#include "singlefunction.h"

SingleFunction::SingleFunction(SystemHandler* handler, FUNCTION_TYPE type, std::string name, std::vector<VarType> argument_types, VarType return_type) : Function(handler, type, name)
{
    this->argument_types = argument_types;
    this->return_type = return_type;
}

SingleFunction::~SingleFunction()
{
}

