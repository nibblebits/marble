#include "value.h"
#include "array.h"
#include "object.h"
#include <stdexcept>
Value::Value()
{
    this->type = -1;
    this->holder = NULL;
    this->ovalue = NULL;
    this->avalue = NULL;
}

Value::~Value()
{

}


VALUE_TYPE Value::getValueTypeForString(std::string str)
{
    if (str == "number")
        return VALUE_TYPE_NUMBER;
    else if(str == "string")
        return VALUE_TYPE_STRING;
    else
        return VALUE_TYPE_OBJECT;
}

void Value::set(Value* v)
{
    this->type = v->type;
    this->holder = v->holder;
    this->ovalue = v->ovalue;
    this->avalue = v->avalue;
    this->dvalue = v->dvalue;
}

bool Value::isObjectOrArray()
{
    return this->type == VALUE_TYPE_OBJECT || this->type == VALUE_TYPE_ARRAY;
}

Value Value::operator+(const Value& other)
{
	ensure_same_type(type, other.type);
	Value value;
	value.type = type;

    switch(type)
    {
        case VALUE_TYPE_NUMBER:
        {
            value.dvalue = dvalue + other.dvalue;
        }
        break;
        case VALUE_TYPE_STRING:
        {
            value.svalue = svalue + other.svalue;    
        }
        break;
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


Value Value::operator>(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue > other.dvalue;
    return value;
}

Value Value::operator<(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue < other.dvalue;
    return value;
}

Value Value::operator>=(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue >= other.dvalue;
    return value;
}

Value Value::operator<=(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue <= other.dvalue;
    return value;
}

Value Value::operator==(const Value& other)
{
	ensure_same_type(type, other.type);
	Value value;
	value.type = VALUE_TYPE_NUMBER;

    switch(type)
    {
        case VALUE_TYPE_NUMBER:
        {
            value.dvalue = dvalue == other.dvalue;
        }
        break;
        case VALUE_TYPE_STRING:
        {
            value.dvalue = svalue == other.svalue;    
        }
        break;
        case VALUE_TYPE_OBJECT:
        case VALUE_TYPE_ARRAY:
        {
            value.dvalue = ovalue == other.ovalue;
        }
        break;
        default:
            throw std::logic_error("Unexpected issue when compareing strings, this is a bug please report this");
    }
    
    return value;
}
Value Value::operator!=(const Value& other)
{
	ensure_same_type(type, other.type);
	Value value;
	value.type = VALUE_TYPE_NUMBER;

    switch(type)
    {
        case VALUE_TYPE_NUMBER:
        {
            value.dvalue = dvalue != other.dvalue;
        }
        break;
        case VALUE_TYPE_STRING:
        {
            value.dvalue = svalue != other.svalue;    
        }
        break;
        case VALUE_TYPE_OBJECT:
        case VALUE_TYPE_ARRAY:
        {
            value.dvalue = ovalue != other.ovalue;
        }
        break;
        default:
            throw std::logic_error("Unexpected issue when compareing strings, this is a bug please report this");
    }
    
    return value;
}
Value Value::operator&&(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue && other.dvalue;
    return value;
}

Value Value::operator||(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue || other.dvalue;
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
	   throw std::logic_error("void Value::ensure_same_type(int type1, int type2): Cannot preform mathematical operator as the values provided differ in type: " + std::to_string(type1) + ":" + std::to_string(type2));
	}
}
