#include "function.h"
#include "interpreter.h"
#include "writtenfunction.h"
#include "nodes.h"
#include "token.h"
#include "posinfo.h"
#include "systemhandler.h"
#include "exceptions/systemexception.h"

Function::Function(FUNCTION_TYPE type, std::string name)
{
    this->type = type;
    this->name = name;
    this->is_pure = false;
}

Function::~Function()
{
}

void Function::invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Now that a function has been invoked we are currently no longer accessing an object
    interpreter->setCurrentObject(NULL, NULL, NULL);

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
        this->invoke_impl(interpreter, values, return_value, object);
    }
    catch(SystemException& ex)
    {
        // An exception was thrown! Lets pop us off the stack trace and rethrow it
        interpreter->popFromStackTrace();

        // We are done with our function
        f_system->finishCurrentFunction();

        // We are done with our NULL current object
        interpreter->finishCurrentObject();

        throw ex;
    }
    
    // Now lets pop it off
    interpreter->popFromStackTrace();

    // We are done with our function
    f_system->finishCurrentFunction();

    // We are done with our NULL current object
    interpreter->finishCurrentObject();
}

std::string Function::getName()
{
    return this->name;
}

