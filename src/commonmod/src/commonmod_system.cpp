#include "commonmod_system.h"
#include "interpreter.h"
#include "exceptions/systemexception.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "exceptionobject.h"
#include "function.h"
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

    // Register the timeout permission
    moduleSystem->getClassSystem()->registerClass("TimeoutPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
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
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the TimeoutPermission which is required for handling timeouts");
        }
    }
    interpreter->setTimeout(values[0].dvalue);
}