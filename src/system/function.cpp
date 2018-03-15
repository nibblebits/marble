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

    Scope* old_scope;
    
    /*
    * If this function is invoked inside an object then it is very important to set the current scope to the object scope
    * to avoid invalid scopes when calling a function.
    */
    if (object != NULL)
    {
        old_scope = interpreter->getCurrentScope();
        interpreter->setCurrentScope(object.get());
    }

    try
    {
        this->invoke_impl(interpreter, values, return_value, object, caller_scope);
    }
    catch(SystemException& ex)
    {
        if (object != NULL)
        {
            // Let's restore the scope
            interpreter->setCurrentScope(old_scope);
        }
        // An exception was thrown! Lets pop us off the stack trace and rethrow it
        interpreter->popFromStackTrace();

        // We are done with our function
        f_system->finishCurrentFunction();

        throw ex;
    }
    

    if (object != NULL)
    {
        // Let's restore the scope
        interpreter->setCurrentScope(old_scope);
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

