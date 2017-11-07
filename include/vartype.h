#ifndef VARTYPE_H
#define VARTYPE_H
#include <string>
#include "statics.h"
class VarType
{
public:
    VarType();
    virtual ~VarType();
    bool operator==(const VarType &other) const;
    bool operator!=(const VarType &other) const;
    static VarType fromString(std::string value);
    VARIABLE_TYPE type;
    std::string value;
};

#endif
