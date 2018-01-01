#include "function.h"
#include "interpreter.h"
#include "writtenfunction.h"
#include "nodes.h"
#include "token.h"
#include "posinfo.h"
#include "systemhandler.h"
#include "exceptions/systemexception.h"

Function::Function(SystemHandler* handler, FUNCTION_TYPE type, std::string name)
{
    this->sys_handler = handler;
    this->type = type;
    this->name = name;
}

Function::~Function()
{
}

SystemHandler* Function::getSystemHandler() {
    return this->sys_handler;
}

void Function::invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Lets keep a stack trace of this function call
    std::string address = (object != NULL ? object->getClass()->name : "Global") + "." + this->name;
    PosInfo posInfo;
    if (this->type == FUNCTION_TYPE_WRITTEN)
    {
        // This is a written function lets get the interpreters last function call node's position
        posInfo = interpreter->getLastFunctionCallNode()->posInfo;
    }
    
    interpreter->addToStackTrace(address, posInfo);
    try
    {
        FunctionSystem* function_system = this->sys_handler->getFunctionSystem();
        function_system->setCurrentFunction(this);
        this->invoke_impl(interpreter, values, return_value, object);
        function_system->finishCurrentFunction();
    }
    catch(SystemException& ex)
    {
        // An exception was thrown! Lets pop us off the stack trace and rethrow it
        interpreter->popFromStackTrace();
        throw ex;
    }
    
    // Now lets pop it off
    interpreter->popFromStackTrace();
}

std::string Function::getName()
{
    return this->name;
}

