#ifndef INTERPRETABLE_H
#define INTERPRETABLE_H

#include "statics.h"
#include "value.h"
class Interpreter;
class Interpretable
{
public:
    virtual Value interpret(Interpreter* interpreter) = 0;
};

#endif
