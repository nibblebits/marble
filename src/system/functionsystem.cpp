#include "functionsystem.h"
#include "function.h"
#include "nativefunction.h"


FunctionSystem::FunctionSystem()
{
    this->prev_fc_sys = NULL;
    this->currentObj = NULL;
}


FunctionSystem::~FunctionSystem()
{

}

Function* FunctionSystem::registerFunction(std::string name, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint)
{
    if (hasFunction(name))
    {
        throw std::logic_error("Function* FunctionSystem::registerFunction(std::string name, std::function<Value(std::vector<Value>)> entrypoint): The function you are trying to register already exists");
    }

    Function* function = new NativeFunction(name, entrypoint);
    this->functions.push_back(std::unique_ptr<Function>(function));
    return function;
}

bool FunctionSystem::hasFunction(std::string name)
{
    return getFunctionByName(name) != NULL;
}

Function* FunctionSystem::getFunctionByName(std::string name)
{
    for (int i = 0; i < this->functions.size(); i++)
    {
        Function* function = this->functions.at(i).get();
        if (function->name == name)
        {
            return function;
        }
    }

    if (this->prev_fc_sys != NULL)
        return this->prev_fc_sys->getFunctionByName(name);
    return NULL;
}
