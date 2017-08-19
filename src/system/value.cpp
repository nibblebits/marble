#include "value.h"
#include <stdexcept>
Value::Value()
{
    this->holder = NULL;
}
Value::~Value()
{

}

Value Value::operator+(const Value& other)
{
	ensure_same_type(type, other.type);
	Value value;
	value.type = type;
    if (type == VALUE_TYPE_NUMBER)
	{
        value.dvalue = dvalue + other.dvalue;
	}

    return value;
}

Value Value::operator-(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
	Value value;
	value.type = type;
    value.dvalue = dvalue - other.dvalue;
    return value;
}

Value Value::operator*(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
	Value value;
	value.type = type;
    value.dvalue = dvalue * other.dvalue;
    return value;
}

Value Value::operator/(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
	Value value;
	value.type = type;
    value.dvalue = dvalue / other.dvalue;
    return value;
}

bool Value::hasHolder()
{
    return this->holder != NULL;
}


void Value::ensure_valid_type(int type, std::vector<int> valid_types)
{
    for (int i : valid_types)
    {
        if (i == type) 
            return;
    }

    throw std::logic_error("void Value::ensure_valid_type(int type, std::vector<int> valid_types): Invalid type provided"); 
}
void Value::ensure_same_type(int type1, int type2)
{
	if (type1 != type2)
	{
	   throw std::logic_error("void Value::ensure_same_type(int type1, int type2): Cannot preform mathematical operator as the values provided differ in type");
	}
}