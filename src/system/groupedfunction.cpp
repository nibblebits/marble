#include "groupedfunction.h"
#include "singlefunction.h"
#include "csystem.h"
#include "systemhandler.h"
#include "statics.h"
GroupedFunction::GroupedFunction(std::string name, SystemHandler* sys_handler) : Function(FUNCTION_TYPE_GROUPED, name)
{

}

GroupedFunction::~GroupedFunction()
{

}


void GroupedFunction::invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    Function* function = getFunctionForValues(values);
    if (function == NULL)
        throw std::logic_error("No function found to invoke for the given values");
    
    function->invoke(values, return_value, object);
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
            std::cout << "fail typed" << std::endl;
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

void GroupedFunction::addFunction(std::unique_ptr<Function> function)
{
    this->functions.push_back(std::move(function));
}
