#include "nativefunction.h"
NativeFunction::NativeFunction(std::string name, std::function<void(std::vector<Value>, Value* return_value, std::shared_ptr<Object> object)> entrypoint) : Function(name)
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
