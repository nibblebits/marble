#ifndef INTERPRETABLE_H
#define INTERPRETABLE_H

#include "statics.h"
#include "value.h"
#include "testable.h"
class Interpreter;
class Interpretable : public Testable
{
public:
    virtual void test(Validator* validator) {} 
    virtual Value interpret(Interpreter* interpreter) = 0;
};

#endif
