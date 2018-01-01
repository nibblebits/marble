#include "nativefunction.h"
NativeFunction::NativeFunction(SystemHandler* handler, std::string name, std::vector<VarType> argument_types, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint) : SingleFunction(handler, FUNCTION_TYPE_NATIVE, name, argument_types, return_type)
{
    this->entrypoint = entrypoint;
}
NativeFunction::~NativeFunction()
{

}
void NativeFunction::invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    if (this->entrypoint == NULL)
    {
        throw std::logic_error("Cannot invoke function " + this->name + " because it has no entrypoint");
    }
    entrypoint(interpreter, values, return_value, object);
}
