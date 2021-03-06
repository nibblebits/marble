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

#ifndef GROUPEDFUNCTION_H
#define GROUPEDFUNCTION_H

#include <memory>
#include <algorithm>
#include "function.h"
#include "vartype.h"

/**
* A grouped function is a function that holds other functions with the same name.
* This is possible with function overloading. Take the following example
* 
* @code
* function getName() : string { return this.name; }
* function getName(number size) { string new_name = ""; for (number i = 0; i < size; i+=1) { new_name += this.name[i]; } } 
* @endcode
*
* As you can see from the two functions named "getName" one of these functions returns the whole name and one of them returns the name up to a certain size.
* This is function overloading as both functions have the same name but different arguments and in the marble interpreter
* when registering a function with the same name with different arguments the function becomes a grouped function and all the different types of functions become functions
* in this grouped function.
*
* <b>Upon invoking a GroupedFunction the GroupedFunction will automatically pick the correct function to invoke based on the input values this allows all 
* functions to all be treated singular </b>
*/

class SystemHandler;
class SingleFunction;
class GroupedFunction : public Function
{
public:
    GroupedFunction(std::string name);
    virtual ~GroupedFunction();
    /**
    * Invokes the correct function in this grouped function based on the values provided. If no function is found with the appropriate
    * arguments for the input values then an exception is thrown.
    *
    * \param values The values to pass to the function to be called
    * \param return_value The return value for this function.
    * \param object The object this function was invoked on. Set to NULL if no object was provided.
    * \param caller_scope The scope of the caller of this function
    * \throw std::logic_error Thrown if no function can be found that can deal with the values provided.
    */
    virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    /**
     * Gets a function based on the values provided
     * \param calling_handler This is the SystemHandler that is used currently, this could be the interpreter instance for example
     */
    Function* getFunctionForValues(std::vector<Value> values, SystemHandler* calling_handler);
    /**
     * Gets a function based on the values provided
     */
    Function* getFunctionForArguments(std::vector<VarType> types);
    /**
     * Returns weather or not a function exists with the given arguments
     */
    bool hasFunctionWithArguments(std::vector<VarType> types);
    void addFunction(std::unique_ptr<Function> function);
    std::vector<Function*> getFunctions();

    // Assists in sorting functions based on type. This had to be done as otherwise std::sort requires a static function we need access to this instance
    bool sort_comparator(SystemHandler* calling_handler, SingleFunction* f1, SingleFunction* f2);

    virtual std::string toString();


    /** The SystemHandler related to this function */
    SystemHandler* sys_handler;
    
private:
    bool isValidFunctionForValues(SingleFunction* function, std::vector<Value> values, SystemHandler* calling_handler);
    std::vector<std::unique_ptr<Function>> functions; 
};

#endif
