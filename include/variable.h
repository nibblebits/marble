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

#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include <memory>
#include "value.h"
class Scope;
class Object;
class Variable
{
public:
    Variable();
    virtual ~Variable();
    static int getVariableTypeForString(std::string str);
    static Variable getFromPointer(Variable* variable);
    bool isArray();
    std::string name;
    VARIABLE_TYPE type;
    std::string type_name;
    
    // The total array dimensions for this variable. Zero if not an array.
    int dimensions;
    
    // The scope this is a part of. For example if this variable is declared in an IF statement the scope will be the IF statement scope.
    Scope* scope;
    
    // The object this variable is a part of. NULL if there is no object.
    Object* object;
    
    // Is this a private, protected or public variable?
    MODIFIER_ACCESS access;
    struct Value value;

    /** 
     * When set to true setValue will throw an exception and refuse to set the variable value
     */
    bool is_locked;
    
    virtual void setValue(Value value, Interpreter* interpreter=NULL);
    void setValue(double value, Interpreter* interpreter=NULL);
    void setValue(std::string value, Interpreter* interpreter=NULL);
    void setValue(std::shared_ptr<Object> value);
   
};
#endif
