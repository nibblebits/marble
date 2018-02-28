#ifndef INTERPRETABLE_H
#define INTERPRETABLE_H

#include "statics.h"
#include "value.h"
#include "testable.h"
#include "extras.h"
#include <iostream>
class Interpreter;
class Interpretable : public Testable
{
public:
    virtual void test(Validator* validator, struct extras extra={}) {} 
    virtual Value interpret(Interpreter* interpreter, struct extras extra={}) = 0;
};

#endif
