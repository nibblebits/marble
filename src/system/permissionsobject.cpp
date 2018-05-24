#include "permissionsobject.h"
#include "permissionobject.h"
#include "systemhandler.h"
#include "interpreter.h"
#include "scope.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <memory>
#include <algorithm>
#include <vector>
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


std::vector<std::shared_ptr<PermissionObject>> PermissionsObject::getPermissionList(std::string name)
{
    std::vector<std::shared_ptr<PermissionObject>> obj_list;
    for (std::shared_ptr<PermissionObject> o : this->objects)
    {
        if (o->getClass()->name == name)
            obj_list.push_back(o);
    }

    return obj_list;
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

    permissions_class->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("Permission"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        PermissionsObject_Get(interpreter, arguments, return_value, object, caller_scope);
    });
    
    permissions_class->registerFunction("remove", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        PermissionsObject_Remove(interpreter, arguments, return_value, object, caller_scope);
    });
    return permissions_class;
}

void PermissionsObject::PermissionsObject_Add(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionsObject> this_p_obj = std::dynamic_pointer_cast<PermissionsObject>(object);
    std::shared_ptr<PermissionObject> permission_to_add = std::dynamic_pointer_cast<PermissionObject>(arguments[0].ovalue);
    if (permission_to_add == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("NullPointerException"))));

    // If the interpreter has no permission restrictions we don't need to validate these permissions before adding them
    if (!interpreter->hasNoPermissionRestrictions())
    {
        /* Does the caller scope already have the permission it is trying to add? If not then it is not legal for this caller to add this permission 
        * as allowing this will defeat the purpose of the permission system in the first place*/
        std::shared_ptr<PermissionObject> caller_permission = caller_scope->permissions->getPermission(permission_to_add->getClass()->name);
        if (caller_permission == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))));
        }

        /**
         * Since the caller scope does have this permission we need to now ensure the variables match and if not then we have to call the permission check function
         * on the PermissionObject caller permission */
        caller_permission->ensurePermissionValid(interpreter, permission_to_add, caller_scope);
    }
    this_p_obj->addPermission(permission_to_add);
}

void PermissionsObject::PermissionsObject_Get(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionsObject> this_p_obj = std::dynamic_pointer_cast<PermissionsObject>(object);
    return_value->type = VALUE_TYPE_OBJECT;
    return_value->ovalue = this_p_obj->getPermission(arguments[0].svalue);
}

void PermissionsObject::PermissionsObject_Remove(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionsObject> p_obj = std::dynamic_pointer_cast<PermissionsObject>(object);
    p_obj->removePermission(arguments[0].svalue);
}