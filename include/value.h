/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef VALUE_H
#define VALUE_H

#include "statics.h"
#include <string>
#include <vector>
#include <memory>
class Variable;
class Array;
class Object;
class Interpreter;
class Value
{
public:
    Value();
    Value(Value* value);
    Value(std::shared_ptr<Object> object);
    Value(std::string str);
    Value(double number);
    virtual ~Value();
    static VALUE_TYPE getValueTypeForString(std::string str);
    static VALUE_TYPE getValueTypeFromVariableType(VARIABLE_TYPE type);
    static std::string getValueStringForType(VALUE_TYPE type);
    void set(Value* v);
    void set(std::string s);
    void set(double v);
    void set(std::shared_ptr<Object> o);

    /**
     * Gets a string for the Value provided regardless of type
     * \param from The Value to get the string from 
     * \param interpreter The interpreter for this instance
     */
    static std::string getStringValue(Value* from, Interpreter* interpreter=NULL);
    /**
     * Gets a double for the Value provided regardless of type
     * on failure to convert to double an exception is thrown.
     * \param from The value to get the double from
     * \param interpreter The interpreter for this instance
     */
    static double getDoubleValue(Value* from, Interpreter* interpreter=NULL);

    VALUE_TYPE type;
    
    // Holds the string type of the value. So for example string, number, int, Object, Dog, Cat
    std::string type_str;
    
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
    Value operator%(const Value& other);
    Value operator&(const Value& other);
    Value operator|(const Value& other);
    Value operator<<(const Value& other);
    Value operator>>(const Value& other);
    Value operator^(const Value& other);

    Value operator>(const Value& other);
    Value operator<(const Value& other);
    Value operator>=(const Value& other);
    Value operator<=(const Value& other);
    bool operator==(const Value& other);
    bool operator!=(const Value& other);
    Value operator&&(const Value& other);
    Value operator||(const Value& other);
    bool hasHolder();
private:
    void ensure_same_type(int type1, int type2);
    void ensure_valid_type(int type, std::vector<int> valid_types);
};

#endif
