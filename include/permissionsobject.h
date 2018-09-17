/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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