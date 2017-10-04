#ifndef ARRAY_H
#define ARRAY_H
#include "object.h"
class Variable;
class Array : public Object
{
public:
    Array(Interpreter* interpreter, Class* c, Variable* variables, int count);
    virtual ~Array();
    virtual void setup();
    Variable* variables;
    int count;
};
#endif
