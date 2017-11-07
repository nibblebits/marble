#include "function.h"
#include "interpreter.h"
#include "systemhandler.h"

Function::Function(SystemHandler* handler, FUNCTION_TYPE type, std::string name)
{
    this->sys_handler = handler;
    this->type = type;
    this->name = name;
}

Function::~Function()
{
}

SystemHandler* Function::getSystemHandler() {
    return this->sys_handler;
}

void Function::invoke(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    FunctionSystem* function_system = this->sys_handler->getFunctionSystem();
    function_system->setCurrentFunction(this);
    this->invoke_impl(values, return_value, object);
    function_system->finishCurrentFunction();
}

std::string Function::getName()
{
    return this->name;
}

