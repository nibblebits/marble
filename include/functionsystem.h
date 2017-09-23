#ifndef FUNCTIONSYSTEM_H
#define FUNCTIONSYSTEM_H

#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>
#include "value.h"
class Function;
class FunctionSystem
{
    public:
        FunctionSystem();
        virtual ~FunctionSystem();
        
        // For native C++ marble functions
        Function* registerFunction(std::string name, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint);
        bool hasFunction(std::string name);
        Function* getFunctionByName(std::string name);
        FunctionSystem* prev_fc_sys;
        std::shared_ptr<Object> currentObj;
    private:
        std::vector<std::unique_ptr<Function>> functions;
};
#endif
