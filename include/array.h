#ifndef ARRAY_H
#define ARRAY_H
#include "object.h"
class ObjectManager;
class Variable;
class Array : public Object
{
public:
    Array(ObjectManager* objectManager, Variable* variables, int count);
    virtual ~Array();
    Variable* variables;
    int count;
};
#endif
