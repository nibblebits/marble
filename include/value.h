#ifndef VALUE_H
#define VALUE_H

#include "statics.h"
#include <string>
#include <vector>
class Variable;
class Array;
class Object;
class Value
{
public:
    Value();
    Value(const Value& value);
    virtual ~Value();
    VALUE_TYPE type;
    // Has this value been activated? This is important and without activation a run-time error will be thrown, call activate();
    bool activated;
    // The variable who holds this value (NULL if the value stands without a variable)
    Variable* holder;

    std::string svalue;
    union
    {
        double dvalue;
        Object* ovalue;
        Array* avalue;
    };
    
    bool isObjectOrArray();
    void activate();
    
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
