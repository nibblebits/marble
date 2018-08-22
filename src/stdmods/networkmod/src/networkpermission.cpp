#include "networkpermission.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"

NetworkPermission::NetworkPermission(Class* c) : PermissionObject(c)
{

}

NetworkPermission::~NetworkPermission()
{

}

std::shared_ptr<Object> NetworkPermission::newInstance(Class* c)
{
    return std::make_shared<NetworkPermission>(c);
}

void NetworkPermission::ensurePermission(Interpreter* interpreter, Scope* caller_scope)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {
        std::shared_ptr<NetworkPermission> permission = std::dynamic_pointer_cast<NetworkPermission>(caller_scope->getPermission("NetworkPermission"));
        if (permission == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the NetworkPermission which is required for network operations");
        }
    }

}
void NetworkPermission::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("NetworkPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<NetworkPermission>(c));

    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

}