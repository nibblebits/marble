#ifndef FUNCTIONSYSTEM_H
#define FUNCTIONSYSTEM_H

#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>
#include "value.h"
class FunctionNode;
class Function;
class Interpreter;
class FunctionSystem
{
    public:
        FunctionSystem();
        FunctionSystem(Interpreter* interpreter);
        FunctionSystem(Interpreter* interpreter, FunctionSystem* prev_fc_sys);
        virtual ~FunctionSystem();
        void setInterpreter(Interpreter* interpreter);
        // For native C++ marble functions
        Function* registerFunction(std::string name, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint);
        Function* registerFunction(FunctionNode* fnode);
        bool hasFunction(std::string name);
        bool hasFunctionLocally(std::string name);
        Function* getFunctionByName(std::string name);
        FunctionSystem* prev_fc_sys;
        std::shared_ptr<Object> currentObj;
    private:
        std::vector<std::unique_ptr<Function>> functions;
        Interpreter* interpreter;
};
#endif
