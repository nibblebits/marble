#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include "value.h"
class Variable
{
public:
    Variable();
    virtual ~Variable();
    static int getVariableTypeForString(std::string str);
    std::string name;
    int type;
    struct Value value;
};
#endif
