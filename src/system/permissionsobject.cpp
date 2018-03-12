#include "permissionsobject.h"
#include "permissionobject.h"
#include "systemhandler.h"
#include "interpreter.h"
#include "scope.h"
#include "exceptions/systemexception.h"
#include <memory>
#include <algorithm>
PermissionsObject::PermissionsObject(Class* c) : Object(c)
{

}

PermissionsObject::~PermissionsObject()
{
    
}

std::shared_ptr<Object> PermissionsObject::newInstance(Class* c)
{
    return std::make_shared<PermissionsObject>(c);
}

void PermissionsObject::addPermission(std::shared_ptr<PermissionObject> permission)
{
    if (permission == NULL)
        throw std::logic_error("NULL permissions are not allowed");
    
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

void PermissionsObject::removePermission(std::string name)
{
    for (std::shared_ptr<PermissionObject> o : this->objects)
    {
        // The object may have been reset so check here
        if (o == NULL)
            continue;

        if (o->getClass()->name == name)
            this->objects.erase(std::remove(this->objects.begin(), this->objects.end(), o));
    }   
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
    
    permissions_class->registerFunction("remove", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        PermissionsObject_Remove(interpreter, arguments, return_value, object, caller_scope);
    });
    return permissions_class;
}

void PermissionsObject::PermissionsObject_Add(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionsObject> p_obj = std::dynamic_pointer_cast<PermissionsObject>(object);
    /* Does the caller scope already have the permission it is trying to add? If not then it is not legal for this caller to add this permission 
     * as allowing this will defeat the purpose of the permission system in the first place*/
    std::shared_ptr<PermissionObject> permission = caller_scope->permissions->getPermission(arguments[0].ovalue->getClass()->name);
    if (permission == NULL)
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException")));

    p_obj->addPermission(std::dynamic_pointer_cast<PermissionObject>(arguments[0].ovalue));
}

void PermissionsObject::PermissionsObject_Remove(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionsObject> p_obj = std::dynamic_pointer_cast<PermissionsObject>(object);
    p_obj->removePermission(arguments[0].svalue);
}