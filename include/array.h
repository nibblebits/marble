#ifndef ARRAY_H
#define ARRAY_H
#include "object.h"
class Variable;
class Array : public Object
{
public:
    Array(Class* c, Variable* variables, int count);
    virtual ~Array();
    Variable* variables;
    int count;
};
#endif
