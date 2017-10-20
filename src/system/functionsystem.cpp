#include "functionsystem.h"
#include "function.h"
#include "nativefunction.h"
#include "writtenfunction.h"
#include "interpreter.h"
#include "nodes.h"
FunctionSystem::FunctionSystem() : FunctionSystem(NULL, NULL)
{

}

FunctionSystem::FunctionSystem(SystemHandler* sys_handler) : FunctionSystem(sys_handler, NULL)
{
}

FunctionSystem::FunctionSystem(SystemHandler* sys_handler, FunctionSystem* prev_fc_sys)
{
    this->sys_handler = sys_handler;
    this->prev_fc_sys = prev_fc_sys;
}


FunctionSystem::~FunctionSystem()
{

}

void FunctionSystem::setSystemHandler(SystemHandler* sys_handler)
{
    this->sys_handler = sys_handler;
}

void FunctionSystem::setPreviousFunctionSystem(FunctionSystem* prev_fc_sys)
{
    this->prev_fc_sys = prev_fc_sys;
}

FunctionSystem* FunctionSystem::getPreviousFunctionSystem()
{
    return this->prev_fc_sys;
}

Function* FunctionSystem::registerFunction(std::string name, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint)
{
    if (hasFunctionLocally(name))
    {
        throw std::logic_error("Function* FunctionSystem::registerFunction(std::string name, std::function<Value(std::vector<Value>)> entrypoint): The function you are trying to register already exists");
    }

    // Blank argument types for now.
    std::vector<VarType> argument_types;
    
    Function* function = new NativeFunction(name, argument_types, entrypoint);
    this->functions.push_back(std::unique_ptr<Function>(function));
    return function;
}

Function* FunctionSystem::registerFunction(FunctionNode* fnode)
{
    if (this->sys_handler == NULL)
        throw std::logic_error("The function system does not have a system handler attached");
    
    if (this->sys_handler->getType() != SYSTEM_HANDLER_INTERPRETER)
        throw std::logic_error("The function: " + fnode->name + " cannot be registered as the current system handler is not an interpreter");
        
    if (hasFunctionLocally(fnode->name))
    {
        // So we already have this function registered so we may need to create a grouped function if it is not that already
        throw std::logic_error("Function* FunctionSystem::registerFunction(FunctionNode* fnode): The function you are trying to register already exists");
    }
    
    
    // We must get the variable arguments and give them to the function we are about to create.
    std::vector<VarType> var_types;
    for (VarNode* node : fnode->args)
    {
        struct Evaluation evaluation = node->type->evaluate(this->sys_handler, EVALUATION_TYPE_DATATYPE);
        var_types.push_back(evaluation.datatype);
    }
    
    Function* function = new WrittenFunction((Interpreter*) sys_handler, fnode, var_types);
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
