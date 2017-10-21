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
    GroupedFunction(std::string name, SystemHandler* sys_handler);
    virtual ~GroupedFunction();
    /**
    * Invokes the correct function in this grouped function based on the values provided. If no function is found with the appropriate
    * arguments for the input values then an exception is thrown.
    *
    * \param values The values to pass to the function to be called
    * \param return_value The return value for this function.
    * \param object The object this function was invoked on. Set to NULL if no object was provided.
    * \throw std::logic_error Thrown if no function can be found that can deal with the values provided.
    */
    virtual void invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    Function* getFunctionForValues(std::vector<Value> values);
    Function* getFunctionForArguments(std::vector<VarType> types);
    bool hasFunctionWithArguments(std::vector<VarType> types);
    void addFunction(std::unique_ptr<Function> function);
    SystemHandler* sys_handler;
private:
    bool isValidFunctionForValues(SingleFunction* function, std::vector<Value> values);
    std::vector<std::unique_ptr<Function>> functions; 
};

#endif
