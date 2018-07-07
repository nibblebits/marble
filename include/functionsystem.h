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
#include "statics.h"
#include "typedef_func.h"

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

        /**
         *  Returns the SystemHandler associated with this FunctionSystem.
         *  If none was provided then NULL is returned stating that this FunctionSystem is not related to a SystemHandler and cannot process FunctionNode's
         */
        SystemHandler* getSystemHandler();
        void setPreviousFunctionSystem(FunctionSystem* prev_fc_sys);
        FunctionSystem* getPreviousFunctionSystem();
        /**
        *
        * Tells the FunctionSystem what the current function is. This should be set to the function that is being interpreted or tested
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
        * Returns the current function that is being called or tested
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
         * 
         */
        virtual Function* registerFunction(std::string name, std::vector<VarType> args, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint);
        virtual Function* registerFunction(FunctionNode* fnode);
        bool hasFunction(std::string name, FunctionSystem* final_fs=NULL);
        bool hasFunction(std::string name, std::vector<VarType> args, FunctionSystem* final_fs=NULL, bool allow_compatible=false, SystemHandler* caller_sys_handler=NULL);
        bool hasFunctionLocally(std::string name);
        bool hasFunctionLocally(std::string name, std::vector<VarType> args, bool allow_compatible=false, SystemHandler* caller_sys_handler=NULL);
        /**
         * Returns a function by the given name
         * \name The name of the function
         * \final_fs The final FunctionSystem to check for this Function if all other attempts fail. 
         */
        Function* getFunctionByName(std::string name, FunctionSystem* final_fs=NULL);
        /**
         * Returns a function by the given name and arguments
         * \name The name of the function
         * \args The Function that has the given argument types that you wish to get
         * \final_fs The final FunctionSystem to check for this Function if all other attempts fail. 
         * \caller_sys_handler The SystemHandler of the caller to this function. NULL is legal and if provided the caller system handler will default to this FunctionSystem's own SystemHandler
         */
        Function* getFunctionByNameAndArguments(std::string name, std::vector<VarType> args, FunctionSystem* final_fs=NULL, bool allow_compatible=false, SystemHandler* caller_sys_handler=NULL);
        Function* getFunctionLocallyByNameAndArguments(std::string name, std::vector<VarType> args, bool allow_compatible=false, SystemHandler* caller_sys_handler=NULL);
        std::vector<Function*> getFunctions();
        std::vector<Function*> getPureFunctions();
        
        std::shared_ptr<Object> currentObj; /*!< Set to an Object instance when calling an Object Class Function through an object access expression such as <i>Foo.Bar();</i> \note This is useful for when the right node of an expression needs to know the current object of the left node \attention this should only be used during an object access expression*/
    private:
        GroupedFunction* createOrAddToGroupForFunction(std::unique_ptr<Function> function);
        GroupedFunction* replaceFunctionWithGroup(std::string function_name);
        FunctionSystem* prev_fc_sys; /*!< The previous, parent function system. Set to NULL if no parent exists. \note This is used for classes that extend other classes \see getFunctionByName*/
        std::map<std::string, std::unique_ptr<Function>> functions;
        Function* current_function;
        std::vector<Function*> current_functions;
        // Note that the FunctionSystem does not require a SystemHandler, therefore don't assume its non-null.
        SystemHandler* sys_handler;
};
#endif
