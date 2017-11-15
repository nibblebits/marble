/*! \file */ 
#ifndef FUNCTIONSYSTEM_H
#define FUNCTIONSYSTEM_H

#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include "value.h"
#include "vartype.h"

class SystemHandler;
class FunctionNode;
class Function;
class GroupedFunction;
class Interpreter;

class FunctionSystem
{
    public:
        FunctionSystem();
        FunctionSystem(SystemHandler* sys_handler);
        FunctionSystem(SystemHandler* sys_handler, FunctionSystem* prev_fc_sys);
        virtual ~FunctionSystem();
        void setSystemHandler(SystemHandler* sys_handler);
        SystemHandler* getSystemHandler();
        void setPreviousFunctionSystem(FunctionSystem* prev_fc_sys);
        FunctionSystem* getPreviousFunctionSystem();
        /**
        *
        * Tells the FunctionSystem what the current function is. This should be set to the function that is being called or tested
        * \param function The function to set as this current function
        * 
        * \attention You must call finishCurrentFunction() if setCurrentFunction is used
        */
        void setCurrentFunction(Function* function);
        /**
        * Finishes the current function. This function must be called if setCurrentFunction is called.
        */
        void finishCurrentFunction();
        /**
        *
        * Returns the current function that is being called  or tested
        * \return Function* The current function that is being called or tested
        */
        Function* getCurrentFunction();
        /**
        *
        * Returns true if a function is currently being called or tested
        * \return bool returns true if a function is being called or tested otherwise false.
        */
        bool isInFunction();
        
        // For native C++ marble functions
        /**
         * Creates and registers a NativeFunction into the FunctionSystem and when the function is called
         * the <b>entrypoint</b> lambda function provided will be invoked.
         * \param name The name of the function to create
         * \param args The arguments this function takes.
         * \param return_type The return type this function will return.
         * \param entrypoint The entrypoint of the function.
         */
        virtual Function* registerFunction(std::string name, std::vector<VarType> args, VarType return_type, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint);
        virtual Function* registerFunction(FunctionNode* fnode);
        bool hasFunction(std::string name);
        bool hasFunction(std::string name, std::vector<VarType> args);
        bool hasFunctionLocally(std::string name);
        bool hasFunctionLocally(std::string name, std::vector<VarType> args);
        Function* getFunctionByName(std::string name);
        Function* getFunctionByNameAndArguments(std::string name, std::vector<VarType> args);
        Function* getFunctionLocallyByNameAndArguments(std::string name, std::vector<VarType> args);
        
        std::shared_ptr<Object> currentObj; /*!< Set to an Object instance when calling an Object Class Function through an object access expression such as <i>Foo.Bar();</i> \note This is useful for when the right node of an expression needs to know the current object of the left node \attention this should only be used during an object access expression*/
    private:
        GroupedFunction* replaceFunctionWithGroup(std::string function_name);
        FunctionSystem* prev_fc_sys; /*!< The previous, parent function system. Set to NULL if no parent exists. \note This is used for classes that extend other classes \see getFunctionByName*/
        std::map<std::string, std::unique_ptr<Function>> functions;
        Function* current_function;
        std::vector<Function*> current_functions;
        SystemHandler* sys_handler;
};
#endif
