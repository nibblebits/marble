#include "value.h"
#include <stdexcept>
Value::Value()
{

}
Value::~Value()
{

}

Value Value::operator+(const Value& other)
{
	if (type != other.type)
	{
	   throw std::logic_error("Cannot add values together as values differ in type");
	}
	Value value;
	value.type = type;
    if (type == VALUE_TYPE_NUMBER)
	{
	   if (other.type == VALUE_TYPE_NUMBER)
       {
           value.dvalue = dvalue + other.dvalue;
       }
	}

    return value;
}
