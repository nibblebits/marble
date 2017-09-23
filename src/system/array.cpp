#include "array.h"
#include "variable.h"
#include <iostream>

Array::Array(Class* c, Variable* variables, int count) : Object(c)
{
    this->variables = variables;
    this->count = count;
}


Array::~Array()
{
   delete[] variables;
}




