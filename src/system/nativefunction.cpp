#include "nativefunction.h"
NativeFunction::NativeFunction(std::string name, std::function<void(std::vector<Value>, Value* return_value)> entrypoint) : Function(name)
{
    this->entrypoint = entrypoint;
}
NativeFunction::~NativeFunction()
{

}
void NativeFunction::invoke(std::vector<Value> values, Value* return_value)
{
    entrypoint(values, return_value);
}
