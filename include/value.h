#ifndef VALUE_H
#define VALUE_H

#include "statics.h"
#include <string>
class Value
{
public:
    Value();
    virtual ~Value();
    VALUE_TYPE type;
    std::string svalue;

    Value operator+(const Value& other);

    union
    {
        int dvalue;
        const char* ovalue;
    };
};

#endif
