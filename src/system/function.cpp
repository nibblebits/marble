#include "function.h"
#include "interpreter.h"
#include "writtenfunction.h"
#include "nodes.h"
#include "token.h"
#include "posinfo.h"
#include "systemhandler.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptions/systemexception.h"

Function::Function(FUNCTION_TYPE type, std::string name)
{
    this->type = type;
    this->name = name;
    this->is_pure = false;
    this->access = MODIFIER_ACCESS_PUBLIC;
    this->cls = NULL;
    this->is_operator_overloading = false;
}

Function::~Function()
{
}

void Function::invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    Value fake_value;
    if (return_value == NULL)
    {
        return_value = &fake_value;
    }

    FunctionSystem* f_system = interpreter->getFunctionSystem();
    // We are also in a function
    f_system->setCurrentFunction(this);

    // Lets keep a stack trace of this function call
    std::string address = (object != NULL ? object->getClass()->name : "Global") + "." + this->name;
    PosInfo posInfo;
    if (this->type == FUNCTION_TYPE_WRITTEN)
    {
        /* This is a written function lets get the interpreters last function call node's position.
         * if there is one. As this function may have been called natively*/
        FunctionCallNode* fc_node = interpreter->getLastFunctionCallNode();
        if (fc_node != NULL)
            posInfo = fc_node->posInfo;
    }
    
    interpreter->addToStackTrace(address, posInfo);

    try
    {
        // Lets create a new parented scope for any extras before the invoke
        interpreter->new_parented_scope();
        // Before invoking lets create a variable called caller_permissions that will hold the permissions of the caller of this function
        // The _caller_permissions variable will not be accessible directly as it would not exist at validation time. You can get the permissions
        // by calling getCallerPermissions() from within marble.
        // This function is registered inside the Interpreter class.
        #warning I want to create a better way of doing this, it is not ideal what so ever as we have a bit of a hack here where we check the function isnt getCallerPermissions
        if (this->name != "getCallerPermissions")
        {
            interpreter->getCurrentScope()->createVariable("_caller_permissions", "Permissions", caller_scope->permissions);
        }
        this->invoke_impl(interpreter, values, return_value, object, caller_scope);
        interpreter->finish_parented_scope();
    }
    catch(SystemException& ex)
    {
        // An exception was thrown! Lets pop us off the stack trace and rethrow it
        interpreter->popFromStackTrace();

        // We are done with our function
        f_system->finishCurrentFunction();

        throw ex;
    }
    

    // Now lets pop it off
    interpreter->popFromStackTrace();

    // We are done with our function
    f_system->finishCurrentFunction();

}

std::string Function::getName()
{
    return this->name;
}


 void Function::Blank(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
 {
    // An empty static function used for when a native programmer wants to create a function that points to nothing.
 }