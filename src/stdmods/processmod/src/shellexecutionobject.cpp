#include "../include/shellexecutionobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "misc.h"
#include <iostream>
#include <thread>

ShellExecutionObject::ShellExecutionObject(Class* c) : Object(c)
{

}

ShellExecutionObject::~ShellExecutionObject()
{
        
}

std::shared_ptr<Object> ShellExecutionObject::newInstance(Class* c)
{
    return std::make_shared<ShellExecutionObject>(c);
}

void ShellExecutionObject::registerClass(ModuleSystem* moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("ShellExecution");
    c->setDescriptorObject(std::make_shared<ShellExecutionObject>(c));

    /**
     * @class ShellExecution
     * 
     * Constructs this ShellExecution object. 
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class ShellExecution
     * 
     * Returns the response code for this ShellExecution
     * 
     * function getResponseCode() : number
     */
     c->registerFunction("getResponseCode", {}, VarType::fromString("number"), ShellExecutionObject::getResponseCode);
     
    /**
     * @class ShellExecution
     * 
     * Returns the response body for this ShellExecution
     * 
     * function getResponse() : string
     */
     c->registerFunction("getResponse", {}, VarType::fromString("string"), ShellExecutionObject::getResponse);
     
}

void ShellExecutionObject::getResponseCode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<ShellExecutionObject> shell_exec_obj = std::dynamic_pointer_cast<ShellExecutionObject>(object);
    return_value->set(shell_exec_obj->response_code);   
}

void ShellExecutionObject::getResponse(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<ShellExecutionObject> shell_exec_obj = std::dynamic_pointer_cast<ShellExecutionObject>(object);
    return_value->set(shell_exec_obj->response);   
}
