#include "../include/shellobject.h"
#include "../include/shellexecutionobject.h"
#include "../include/shellpermission.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "misc.h"
#include <iostream>
#include <thread>

ShellObject::ShellObject(Class* c) : Object(c)
{

}

ShellObject::~ShellObject()
{
        
}

std::shared_ptr<Object> ShellObject::newInstance(Class* c)
{
    return std::make_shared<ShellObject>(c);
}

void ShellObject::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class Shell
     * 
     * Responsible for executing shell commands
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Shell");
    c->setDescriptorObject(std::make_shared<ShellObject>(c));

    /**
     * @class ShellObject
     * 
     * Constructs this ShellObject object. 
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class ShellObject
     * 
     * Executes the provided command and returns a ShellExecution object that holds execution state and responses.
     * 
     * You are required to hold a ShellPermision permission in order to execute shell commands
     * 
     * function execute(string command) : ShellExecution
     */
     c->registerFunction("execute", {VarType::fromString("string")}, VarType::fromString("ShellExecution"), ShellObject::Shell_execute);
     
      
}

void ShellObject::Shell_execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Ensure we have permission
    ShellPermission::ensurePermission(interpreter, caller_scope);
    
    // Create the ShellExecution object
    std::shared_ptr<ShellExecutionObject> shell_exec_obj = std::dynamic_pointer_cast<ShellExecutionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("ShellExecution"), {}));
    int exec_return_val = 0;
    std::string response = exec(values[0].svalue, exec_return_val);
    shell_exec_obj->response_code = exec_return_val;
    shell_exec_obj->response = response; 
    return_value->set(shell_exec_obj);
}