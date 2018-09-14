#include "array.h"
#include "variable.h"
#include "interpreter.h"
#include <iostream>

Array::Array(Class* c) : Object(c)
{
    this->variables = NULL;
    this->count = 0;
}

Array::Array(Class* c, Variable* variables, int count) : Object(c)
{
    this->variables = variables;
    this->count = count;
}


Array::~Array()
{
   if (this->variables != NULL)
      delete[] variables;
}

void Array::setup()
{
    // Nothing to setup
}

