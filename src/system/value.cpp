#include "value.h"
#include "array.h"
#include "object.h"
#include <stdexcept>
Value::Value()
{
    this->type = -1;
    this->activated = false;
    this->holder = NULL;
}

Value::Value(const Value& value)
{

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
    
    
    this->type = value.type;
    this->activated = value.activated;
    this->holder = value.holder;
    this->svalue = value.svalue;
    this->dvalue = value.dvalue;
    this->ovalue = value.ovalue;
    this->avalue = value.avalue;
}
Value::~Value()
{
    if (this->type != -1)
    {
        if (!this->activated)
        {
            throw std::logic_error("Value created but was never activated");
        }
        
        if (isObjectOrArray())
        {
            this->ovalue->decrementUsage();
        }
    }
}

bool Value::isObjectOrArray()
{
    return this->type == VALUE_TYPE_OBJECT || this->type == VALUE_TYPE_ARRAY;
}

void Value::activate()
{
    if (this->activated)
    {
        throw std::logic_error("This value has already been activated");
    }
    
    this->activated = true;
    
    if (isObjectOrArray())
    {
        this->ovalue->incrementUsage();
    }
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

    value.activate();
    return value;
}

Value Value::operator-(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
	Value value;
	value.type = type;
    value.dvalue = dvalue - other.dvalue;
    value.activate();
    return value;
}

Value Value::operator*(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
	Value value;
	value.type = type;
    value.dvalue = dvalue * other.dvalue;
    value.activate();
    return value;
}

Value Value::operator/(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
	Value value;
	value.type = type;
    value.dvalue = dvalue / other.dvalue;
    value.activate();
    return value;
}


Value Value::operator>(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue > other.dvalue;
    value.activate();
    return value;
}

Value Value::operator<(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue < other.dvalue;
    value.activate();
    return value;
}

Value Value::operator>=(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue >= other.dvalue;
    value.activate();
    return value;
}

Value Value::operator<=(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue <= other.dvalue;
    value.activate();
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
        default:
            throw std::logic_error("Unexpected issue when compareing strings, this is a bug please report this");
    }
    
    value.activate();
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
        default:
            throw std::logic_error("Unexpected issue when compareing strings, this is a bug please report this");
    }
    
    value.activate();
    return value;
}
Value Value::operator&&(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue && other.dvalue;
    value.activate();
    return value;
}

Value Value::operator||(const Value& other)
{
    ensure_valid_type(type, {VALUE_TYPE_NUMBER});
    ensure_valid_type(other.type, {VALUE_TYPE_NUMBER});
    Value value;
    value.type = VALUE_TYPE_NUMBER;
    value.dvalue = dvalue || other.dvalue;
    value.activate();
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
