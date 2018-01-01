#include "groupedfunction.h"
#include "singlefunction.h"
#include "csystem.h"
#include "systemhandler.h"
#include "statics.h"
GroupedFunction::GroupedFunction(SystemHandler* handler, std::string name) : Function(handler, FUNCTION_TYPE_GROUPED, name)
{
   this->sys_handler = handler;
}

GroupedFunction::~GroupedFunction()
{

}


void GroupedFunction::invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    Function* function = getFunctionForValues(values);
    if (function == NULL)
        throw std::logic_error("No function found to invoke for the given values");
    
    function->invoke(interpreter, values, return_value, object);
}

Function* GroupedFunction::getFunctionForValues(std::vector<Value> values)
{
  for (int i = 0; i < this->functions.size(); i++)
  {
    Function* function = this->functions.at(i).get();
    if (isValidFunctionForValues((SingleFunction*) function, values))
        return function;
  }
  
  return NULL;
}

bool GroupedFunction::isValidFunctionForValues(SingleFunction* function, std::vector<Value> values)
{
    std::vector<VarType> argument_types = function->argument_types; 
    if (argument_types.size() != values.size())
        return false;
    
    for (int i = 0; i < argument_types.size(); i++)
    {
        VarType* arg_type = &argument_types[i];
        // The value equivalent of this variable type. E.g the type "number" and the type "byte" will both be of value type "VALUE_TYPE_NUMBER".
        VALUE_TYPE arg_value_type = Value::getValueTypeFromVariableType(arg_type->type);
        Value* value = &values[i];
        if (arg_value_type != value->type)
        {
            // The argument value type and the value type do not match this means they are incompatible;
            return false;
        }
        
        if (arg_type->type == VARIABLE_TYPE_OBJECT)
        {
            // The variable type provided is an object, we must now check they are of compatible types.
            ClassSystem* class_sys = sys_handler->getClassSystem();
            std::string arg_obj_class_name = arg_type->value;
            Class* arg_obj_class = class_sys->getClassByName(arg_obj_class_name);
            Class* value_obj_class = value->ovalue->getClass();
            if (!value_obj_class->instanceOf(arg_obj_class))
                return false;
        }
    }
    
    return true;
}


Function* GroupedFunction::getFunctionForArguments(std::vector<VarType> types)
{
    for (int i = 0; i < this->functions.size(); i++)
    {
      SingleFunction* function = (SingleFunction*) this->functions.at(i).get();
      if (types == function->argument_types)
          return function;
    }
    return NULL;
}

bool GroupedFunction::hasFunctionWithArguments(std::vector<VarType> types)
{
    return getFunctionForArguments(types) != NULL;
}

void GroupedFunction::addFunction(std::unique_ptr<Function> function)
{
    // This is a bad way of doing it, the vector type should change to std::unique_ptr<SingleFunction>
    if (function->type == FUNCTION_TYPE_GROUPED)
        throw std::logic_error("Only single functions can be added to grouped functions");
    
    SingleFunction* single_function = (SingleFunction*) function.get();
    if (hasFunctionWithArguments(single_function->argument_types))
        throw std::logic_error("A function with the same name and arguments is already registered");
        
    this->functions.push_back(std::move(function));
}
