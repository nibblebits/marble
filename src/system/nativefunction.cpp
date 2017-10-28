#include "nativefunction.h"
NativeFunction::NativeFunction(std::string name, std::vector<VarType> argument_types, VarType return_type, std::function<void(std::vector<Value>, Value* return_value, std::shared_ptr<Object> object)> entrypoint) : SingleFunction(FUNCTION_TYPE_NATIVE, name, argument_types, return_type)
{
    this->entrypoint = entrypoint;
}
NativeFunction::~NativeFunction()
{

}
void NativeFunction::invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    if (this->entrypoint == NULL)
    {
        throw std::logic_error("Cannot invoke function " + this->name + " because it has no entrypoint");
    }
    entrypoint(values, return_value, object);
}
