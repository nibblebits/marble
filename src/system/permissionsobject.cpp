#include "permissionsobject.h"
#include "permissionobject.h"
#include "systemhandler.h"
#include "scope.h"
#include <memory>
PermissionsObject::PermissionsObject(Class* c) : Object(c)
{

}

PermissionsObject::~PermissionsObject()
{
    
}


void PermissionsObject::addPermission(std::shared_ptr<PermissionObject> permission)
{
    this->objects.push_back(permission);
}

std::shared_ptr<PermissionObject> PermissionsObject::getPermission(std::string name)
{
    for (std::shared_ptr<PermissionObject> o : this->objects)
    {
        if (o->getClass()->name == name)
            return o;
    }

    return NULL;
}

Class* PermissionsObject::registerClass(SystemHandler* systemHandler)
{
    Class* permissions_class = systemHandler->getClassSystem()->registerClass("Permissions");
    permissions_class->setDescriptorObject(std::make_shared<PermissionsObject>(permissions_class));
    permissions_class->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });

    permissions_class->registerFunction("add", {VarType::fromString("Permission")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        PermissionsObject_Add(interpreter, arguments, return_value, object, caller_scope);
    });
    
    return permissions_class;
}

void PermissionsObject::PermissionsObject_Add(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionsObject> p_obj = std::dynamic_pointer_cast<PermissionsObject>(object);
    p_obj->addPermission(std::dynamic_pointer_cast<PermissionObject>(arguments[0].ovalue));
}

std::shared_ptr<Object> PermissionsObject::newInstance(Class* c)
{
    return std::make_shared<PermissionsObject>(c);
}