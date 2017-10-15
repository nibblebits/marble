#include "functionsystem.h"
#include "function.h"
#include "nativefunction.h"
#include "writtenfunction.h"
#include "interpreter.h"
#include "fnode.h"

FunctionSystem::FunctionSystem() : FunctionSystem(NULL, NULL)
{

}

FunctionSystem::FunctionSystem(Interpreter* interpreter) : FunctionSystem(interpreter, NULL)
{
}

FunctionSystem::FunctionSystem(Interpreter* interpreter, FunctionSystem* prev_fc_sys)
{
    this->interpreter = interpreter;
    this->prev_fc_sys = prev_fc_sys;
}


FunctionSystem::~FunctionSystem()
{

}

void FunctionSystem::setInterpreter(Interpreter* interpreter)
{
    this->interpreter = interpreter;
}

Function* FunctionSystem::registerFunction(std::string name, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint)
{
    if (hasFunctionLocally(name))
    {
        throw std::logic_error("Function* FunctionSystem::registerFunction(std::string name, std::function<Value(std::vector<Value>)> entrypoint): The function you are trying to register already exists");
    }

    Function* function = new NativeFunction(name, entrypoint);
    this->functions.push_back(std::unique_ptr<Function>(function));
    return function;
}

Function* FunctionSystem::registerFunction(FunctionNode* fnode)
{
    if (hasFunctionLocally(fnode->name))
    {
        throw std::logic_error("Function* FunctionSystem::registerFunction(FunctionNode* fnode): The function you are trying to register already exists");
    }
    
    Function* function = new WrittenFunction(interpreter, fnode);
    this->functions.push_back(std::unique_ptr<Function>(function));
    return function;
}

bool FunctionSystem::hasFunction(std::string name)
{
    return getFunctionByName(name) != NULL;
}

bool FunctionSystem::hasFunctionLocally(std::string name)
{
    for (int i = 0; i < this->functions.size(); i++)
    {
        Function* function = this->functions.at(i).get();
        if (function->name == name)
        {
            return true;
        }
    }
    return false;
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
