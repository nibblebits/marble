/*! \file */ 
#ifndef FUNCTIONSYSTEM_H
#define FUNCTIONSYSTEM_H

#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>
#include "value.h"
class SystemHandler;
class FunctionNode;
class Function;
class Interpreter;

class FunctionSystem
{
    public:
        FunctionSystem();
        FunctionSystem(SystemHandler* sys_handler);
        FunctionSystem(SystemHandler* sys_handler, FunctionSystem* prev_fc_sys);
        virtual ~FunctionSystem();
        void setSystemHandler(SystemHandler* sys_handler);
        // For native C++ marble functions
        /**
         * Creates and registers a NativeFunction into the FunctionSystem and when the function is called
         * the <b>entrypoint</b> lambda function provided will be invoked.
         * \param name The name of the function to create
         * \param entrypoint The entrypoint of the function.
         */
        Function* registerFunction(std::string name, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint);
        Function* registerFunction(FunctionNode* fnode);
        bool hasFunction(std::string name);
        bool hasFunctionLocally(std::string name);
        Function* getFunctionByName(std::string name);
        FunctionSystem* prev_fc_sys; /*!< The previous, parent function system. Set to NULL if no parent exists. \note This is used for classes that extend other classes \see getFunctionByName*/
        std::shared_ptr<Object> currentObj; /*!< Set to an Object instance when calling an Object Class Function through an object access expression such as <i>Foo.Bar();</i> \note This is useful for when the right node of an expression needs to know the current object of the left node \attention this should only be used during an object access expression*/
    private:
        std::vector<std::unique_ptr<Function>> functions;
        SystemHandler* sys_handler;
};
#endif
