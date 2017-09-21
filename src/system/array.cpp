#include "array.h"
#include "variable.h"
#include <iostream>

Array::Array(Variable* variables, int count)
{
    this->variables = variables;
    this->count = count;
}


Array::~Array()
{
    delete[] variables;
}




