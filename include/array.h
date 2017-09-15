#ifndef ARRAY_H
#define ARRAY_H
class Variable;
class Array
{
public:
    Array(Variable* variables, int count);
    virtual ~Array();
    Variable* variables;
    int count;
};
#endif
