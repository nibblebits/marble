#ifndef VALUE_H
#define VALUE_H

#include "statics.h"
#include "cpointers.h"
#include <string>
#include <vector>
#include <memory>
class Variable;
class Array;
class Object;
class Value
{
public:
    Value();
    virtual ~Value();
    static VALUE_TYPE getValueTypeForString(std::string str);
    static VALUE_TYPE getValueTypeFromVariableType(VARIABLE_TYPE type);
    
    static std::string getValueStringForType(VALUE_TYPE type);
    void set(Value* v);
    VALUE_TYPE type;
    // The variable who holds this value (NULL if the value stands without a variable)
    Variable* holder;

    std::string svalue;
    union
    {
        double dvalue;
        char bvalue;
    };
    
    std::shared_ptr<Object> ovalue;
    std::shared_ptr<Array> avalue;
    
    bool isObjectOrArray();
    
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
