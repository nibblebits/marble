#include "shellpermission.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"

ShellPermission::ShellPermission(Class* c) : PermissionObject(c)
{

}

ShellPermission::~ShellPermission()
{

}

std::shared_ptr<Object> ShellPermission::newInstance(Class* c)
{
    return std::make_shared<ShellPermission>(c);
}

void ShellPermission::ensurePermission(Interpreter* interpreter, Scope* caller_scope)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {
        std::shared_ptr<ShellPermission> permission = std::dynamic_pointer_cast<ShellPermission>(caller_scope->getPermission("ShellPermission"));
        if (permission == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the ShellPermission which is required for Shell operations", interpreter->getStackTraceLog());
        }
    }

}
void ShellPermission::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class ShellPermission
     * 
     * If you hold this permission you can execute shell commands
     */ 
    Class* c = moduleSystem->getClassSystem()->registerClass("ShellPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<ShellPermission>(c));

    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);
}