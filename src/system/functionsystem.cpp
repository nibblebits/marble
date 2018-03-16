#include "functionsystem.h"
#include "function.h"
#include "nativefunction.h"
#include "writtenfunction.h"
#include "groupedfunction.h"
#include "purefunction.h"
#include "interpreter.h"
#include "nodes.h"
#include <iostream>
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
    this->current_function = NULL;
}


FunctionSystem::~FunctionSystem()
{

}

void FunctionSystem::setCurrentFunction(Function* function)
{
    this->current_functions.push_back(this->current_function);
    this->current_function = function;
}

void FunctionSystem::finishCurrentFunction()
{
    this->current_function = this->current_functions.back();
    this->current_functions.pop_back();
}

Function* FunctionSystem::getCurrentFunction()
{
    return this->current_function;
}

bool FunctionSystem::isInFunction()
{
    return this->current_function != NULL;
}

void FunctionSystem::setSystemHandler(SystemHandler* sys_handler)
{
    this->sys_handler = sys_handler;
}

SystemHandler* FunctionSystem::getSystemHandler()
{
    return this->sys_handler;
}

void FunctionSystem::setPreviousFunctionSystem(FunctionSystem* prev_fc_sys)
{
    this->prev_fc_sys = prev_fc_sys;
}

FunctionSystem* FunctionSystem::getPreviousFunctionSystem()
{
    return this->prev_fc_sys;
}

GroupedFunction* FunctionSystem::replaceFunctionWithGroup(std::string function_name)
{
    if (this->sys_handler == NULL)
        throw std::logic_error("Grouped functions are not supported for FunctionSystem's with no SystemHandler");
    GroupedFunction* grouped_function = new GroupedFunction(function_name);
    grouped_function->sys_handler = this->sys_handler;
    std::unique_ptr<Function> old_function = std::move(this->functions[function_name]);
    grouped_function->addFunction(std::move(old_function));
    this->functions[function_name] = std::unique_ptr<Function>(grouped_function);
    return grouped_function;
}

Function* FunctionSystem::registerFunction(std::string name, std::vector<VarType> args, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint)
{
    if (hasFunctionLocally(name, args))
    {
        throw std::logic_error("The function: " + name + " has already been registered with the given arguments");
    }

    Function* function = new NativeFunction(name, args, return_type, entrypoint);
    this->functions[name] = std::unique_ptr<Function>(function);
    return function;
}

Function* FunctionSystem::registerFunction(FunctionNode* fnode)
{
    if (this->sys_handler == NULL)
        throw std::logic_error("The function system does not have a system handler attached");
    
    // We must get the variable arguments and give them to the function we are about to create.
    std::vector<VarType> var_types;
    for (VarNode* node : fnode->args)
    {
        struct Evaluation evaluation = node->type->evaluate(this->sys_handler, EVALUATION_TYPE_DATATYPE);
        var_types.push_back(evaluation.datatype);
    }    
    
    VarType return_type;
    struct Evaluation evaluation = fnode->return_type->evaluate(this->sys_handler, EVALUATION_TYPE_DATATYPE);
    return_type = evaluation.datatype;
    
    // Has a function with the same variable argument types already been registered
    if (hasFunctionLocally(fnode->name, var_types))
        throw std::logic_error("The function: " + fnode->name + " has already been registered with the given arguments");
    
    Function* function_raw;
    if (fnode->is_pure)
    {
        // Pure functions can never be interpreted. They are like abstract methods.
        function_raw = new PureFunction(fnode->name, var_types, return_type);
    }
    else
    {
        function_raw = new WrittenFunction(fnode, var_types, return_type);
    }

    std::unique_ptr<Function> function = std::unique_ptr<Function>(function_raw);
    function->access = fnode->access;
    if (hasFunctionLocally(fnode->name))
    {
        // So we already have this function registered so we may need to create a grouped function if it is not that already
        Function* current_func = getFunctionByName(fnode->name);
        GroupedFunction* grouped_func = (GroupedFunction*) current_func;
        if (current_func->type != FUNCTION_TYPE_GROUPED)
        {
           // Ok we need to create a grouped function and move it into this function
           grouped_func = replaceFunctionWithGroup(fnode->name);           
        }
        
        // Now add our new function to this grouped function
        grouped_func->addFunction(std::move(function));
    }
    else
    {
        this->functions[fnode->name] = std::move(function);
    }
    
    return function_raw;
}

bool FunctionSystem::hasFunction(std::string name, FunctionSystem* final_fs)
{
    return getFunctionByName(name, final_fs) != NULL;
}

bool FunctionSystem::hasFunction(std::string name, std::vector<VarType> args, FunctionSystem* final_fs)
{
    return this->getFunctionByNameAndArguments(name, args, final_fs) != NULL;
}

bool FunctionSystem::hasFunctionLocally(std::string name)
{
    return this->functions.find(name) != this->functions.end();
}

bool FunctionSystem::hasFunctionLocally(std::string name, std::vector<VarType> args)
{
    Function* function = this->getFunctionLocallyByNameAndArguments(name, args);
    return function != NULL;
}

Function* FunctionSystem::getFunctionByName(std::string name, FunctionSystem* final_fs)
{
   // If final_fs points to self then we cannot use it.
   if (final_fs == this)
      final_fs = NULL;

   Function* function = NULL;
   
   std::map<std::string, std::unique_ptr<Function>>::iterator it = this->functions.find(name); 
   if (it != this->functions.end())
   {
       function = it->second.get();
       if (function != NULL)
          return function;
   }
    
   if (this->prev_fc_sys != NULL)
   {
        function = this->prev_fc_sys->getFunctionByName(name);
        if (function != NULL)
            return function;
   }
    
   if (final_fs != NULL)
   {
       return final_fs->getFunctionByName(name);
   }
   return NULL;
}

Function* FunctionSystem::getFunctionLocallyByNameAndArguments(std::string name, std::vector<VarType> args)
{
    std::map<std::string, std::unique_ptr<Function>>::iterator it = this->functions.find(name); 
    if (it == this->functions.end())
        return NULL;
        
    Function* function = it->second.get();
    if (function != NULL)
    {
        switch(function->type)
        {
            case FUNCTION_TYPE_WRITTEN:
            case FUNCTION_TYPE_NATIVE:
            case FUNCTION_TYPE_PURE:
            {
                SingleFunction* single_function = (SingleFunction*) function;
                std::vector<VarType> single_function_arguments = single_function->argument_types;
                /**
                 * We must ensure every type is compatible with the types provided to us
                 */
                for (int i = 0; i < single_function_arguments.size(); i++)
                {
                    if (i < args.size())
                    {
                        VarType function_arg = single_function_arguments[i];
                        VarType provided_arg = args[i];
                        if (!provided_arg.ensureCompatibility(function_arg, this->sys_handler->getClassSystem()))
                        {
                            return NULL;
                        }
                    }
                }

                return function;
            }
            break;
            
            case FUNCTION_TYPE_GROUPED:
            {
                GroupedFunction* grouped_function = (GroupedFunction*) function;
                function = grouped_function->getFunctionForArguments(args);      
                return function;
            }
            break;
        } 
        
    }

   return NULL;   
}

Function* FunctionSystem::getFunctionByNameAndArguments(std::string name, std::vector<VarType> args, FunctionSystem* final_fs)
{
   // If final_fs points to self then we cannot use it.
   if (final_fs == this)
      final_fs = NULL;

    Function* function = getFunctionLocallyByNameAndArguments(name, args);
    if (function == NULL)
    { 
        if (this->prev_fc_sys != NULL)
        {
            function = this->prev_fc_sys->getFunctionByNameAndArguments(name, args);
            if (function != NULL)
                return function;
        }
        if (final_fs != NULL)
        {
            return final_fs->getFunctionByNameAndArguments(name, args);
        }
    }
    return function;
}


std::vector<Function*> FunctionSystem::getFunctions()
{
    std::vector<Function*> _functions;
    for (std::map<std::string, std::unique_ptr<Function>>::iterator it = this->functions.begin(); it != this->functions.end(); it++)
    {
        Function* f = it->second.get();
        if (f->type == FUNCTION_TYPE_GROUPED)
        {
            std::vector<Function*> grouped_functions = ((GroupedFunction*)f)->getFunctions();
            _functions.insert(_functions.begin(), grouped_functions.begin(), grouped_functions.end());
        }
        else
            _functions.push_back(f);
    }

    return _functions;
}

std::vector<Function*> FunctionSystem::getPureFunctions()
{
    std::vector<Function*> _functions;
    for (Function* f : getFunctions())
    {
        if (f->is_pure)
        {
            _functions.push_back(f);
        }
    }

    return _functions;
}