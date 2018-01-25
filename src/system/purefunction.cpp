#include "purefunction.h"
PureFunction::PureFunction(std::string function_name, std::vector<VarType> argument_types, VarType return_type) : SingleFunction(FUNCTION_TYPE_PURE, function_name, argument_types, return_type)
{
    this->is_pure = true;
}

PureFunction::~PureFunction()
{

}

void PureFunction::invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    throw std::logic_error("Pure functions cannot be invoked directly. The validator should have stopped this so this is a bug please report it.");
}