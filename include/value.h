#ifndef VALUE_H
#define VALUE_H

#include "statics.h"
#include <string>
#include <vector>
class Variable;
class Value
{
public:
    Value();
    virtual ~Value();
    VALUE_TYPE type;
    // The variable who holds this value (NULL if the value stands without a variable)
    Variable* holder;

    std::string svalue;
    union
    {
        double dvalue;
        Variable* vvalue;    
    };
    Value operator+(const Value& other);
    Value operator-(const Value& other);
    Value operator*(const Value& other);
    Value operator/(const Value& other);
    Value operator>(const Value& other);
    Value operator<(const Value& other);
    Value operator>=(const Value& other);
    Value operator<=(const Value& other);
    Value operator==(const Value& other);
    Value operator!=(const Value& other);
    Value operator&&(const Value& other);
    Value operator||(const Value& other);
    bool hasHolder();
private:
    void ensure_same_type(int type1, int type2);
    void ensure_valid_type(int type, std::vector<int> valid_types);
};

#endif
