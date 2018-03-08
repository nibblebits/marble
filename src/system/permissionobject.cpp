#include "permissionobject.h"
#include "systemhandler.h"
#include <memory>
PermissionObject::PermissionObject(Class* c) : Object(c)
{

}

PermissionObject::~PermissionObject()
{
    
}

Class* PermissionObject::registerClass(SystemHandler* systemHandler)
{
    Class* permission_class = systemHandler->getClassSystem()->registerClass("Permission");
    permission_class->setDescriptorObject(std::make_shared<PermissionObject>(permission_class));
    permission_class->is_pure = true;
    permission_class->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });
    return permission_class;
}

std::shared_ptr<Object> PermissionObject::newInstance(Class* c)
{
    return std::make_shared<PermissionObject>(c);
}