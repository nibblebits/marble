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
   // Causes seg fault, likely because value has object and array both as shared pointer and they are equal to eachother when an array is set
   // delete[] variables;
}




