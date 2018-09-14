#include "commonmod_system.h"
#include "interpreter.h"
#include "exceptions/systemexception.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "exceptionobject.h"
#include "function.h"
#include "array.h"
#include <unistd.h>

CommonModule_System::CommonModule_System(Class* c) : Object(c)
{

}

CommonModule_System::~CommonModule_System()
{
    
}

std::shared_ptr<Object> CommonModule_System::newInstance(Class* c)
{
    return std::make_shared<CommonModule_System>(c);
}

Class* CommonModule_System::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class System
     * 
     * Responsible for providing access to Marble system related entities
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("System");
    c->setDescriptorObject(std::make_shared<CommonModule_System>(c));
    /**
     * @class System
     * constructs this System object
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);
    /**
     * @class System
     * Sets the timeout for this Marble script.
     * A timeout in marble is the amount of miliseconds a script can run before it is forcefully terminated.
     * Set the timeout to zero to disable timeouts.
     * 
     * You are required to hold a TimeoutPermission to change timeouts.
     * 
     * Default usage: System.setTimeout(300);
     * 
     * function setTimeout(number timeout) : void
     */
    c->registerFunction("setTimeout", {VarType::fromString("number")}, VarType::fromString("void"), CommonModule_System::System_SetTimeout);

    /**
     * @class System
     * Changes the working directory.
     * 
     * You are required to hold a ChdirPermission to change the working directory
     * 
     * @works_without_class
     * function chdir(string new_directory) : void
     */
    c->registerFunction("chdir", {VarType::fromString("string")}, VarType::fromString("void"), CommonModule_System::System_chdir);
    moduleSystem->getFunctionSystem()->registerFunction("chdir", {VarType::fromString("string")}, VarType::fromString("void"), CommonModule_System::System_chdir);
  
    // Register the timeout permission
    moduleSystem->getClassSystem()->registerClass("TimeoutPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));

    // Register a ChdirPermission
    moduleSystem->getClassSystem()->registerClass("ChdirPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));

    /**
     * @class System
     *
     * Gets a string array that holds the arguments passed to your application
     * 
     * function getArguments() : string[]
     */
    c->registerFunction("getArguments", {}, VarType::fromString("string[]"), CommonModule_System::System_getArguments);
  
}

void CommonModule_System::newInterpreter(Interpreter* interpreter)
{
    Class* c = interpreter->getClassSystem()->getClassByName("System");
    interpreter->getRootScope()->createVariable("System", "System", Object::create(interpreter, c, {}));
}

void CommonModule_System::System_SetTimeout(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {
        std::shared_ptr<PermissionObject> permission = caller_scope->getPermission("TimeoutPermission");
        if (permission == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the TimeoutPermission which is required for handling timeouts", interpreter->getStackTraceLog());
        }
    }
    interpreter->setTimeout(values[0].dvalue);
}

void CommonModule_System::System_chdir(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {
        std::shared_ptr<PermissionObject> permission = caller_scope->getPermission("ChdirPermission");
        if (permission == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the ChdirPermission which is required for changing the working directory", interpreter->getStackTraceLog());
        }
    }
 
    if(chdir(values[0].svalue.c_str()) != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to change working directory", interpreter->getStackTraceLog());

}

void CommonModule_System::System_getArguments(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Let's return the arguments
    return_value->set(interpreter->getArgs());
}