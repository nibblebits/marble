#include "array.h"
#include "variable.h"
#include "interpreter.h"
#include <iostream>

Array::Array(Interpreter* interpreter, Class* c, Variable* variables, int count) : Object(interpreter, c)
{
    this->variables = variables;
    this->count = count;
}


Array::~Array()
{
   delete[] variables;
}

void Array::setup()
{
    // Nothing to setup
}




