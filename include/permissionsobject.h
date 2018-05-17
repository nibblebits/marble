#ifndef PERMISSIONSOBJECT_H
#define PERMISSIONSOBJECT_H

#include "object.h"
#include <iostream>
#include <vector>

class PermissionObject;
class Scope;
class PermissionsObject : public Object
{
public:
    PermissionsObject(Class* c);
    virtual ~PermissionsObject();

    void addPermission(std::shared_ptr<PermissionObject> permission);
    std::shared_ptr<PermissionObject> getPermission(std::string name);
    std::vector<std::shared_ptr<PermissionObject>> getPermissionList(std::string name);
    
    void removePermission(std::string name);
    static Class* registerClass(SystemHandler* systemHandler);
    virtual std::shared_ptr<Object> newInstance(Class* c);

    std::vector<std::shared_ptr<PermissionObject>> objects;
    
    // Native marble functions
    static void PermissionsObject_Add(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void PermissionsObject_Get(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void PermissionsObject_Remove(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};

#endif