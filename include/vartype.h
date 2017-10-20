#ifndef VARTYPE_H
#define VARTYPE_H
#include <string>
#include "statics.h"
class VarType
{
public:
    VarType();
    virtual ~VarType();
    VARIABLE_TYPE type;
    std::string value;
};

#endif
