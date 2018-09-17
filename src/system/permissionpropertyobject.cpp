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

#include "permissionpropertyobject.h"
#include "systemhandler.h"
PermissionPropertyObject::PermissionPropertyObject(Class* c) : Object(c)
{
    this->name = "";
    this->value = "";
}
PermissionPropertyObject::~PermissionPropertyObject()
{

}

Class* PermissionPropertyObject::registerClass(SystemHandler* systemHandler)
{
    /**
     * class PermissionProperty
     * 
     * A PermissionProperty holds a name and a value and they are used for helping confirm weather or not a scope is allowed to add
     * a Permission to a Permissions object
     */
    Class* permission_property_class = systemHandler->getClassSystem()->registerClass("PermissionProperty");
    permission_property_class->setDescriptorObject(std::make_shared<PermissionPropertyObject>(permission_property_class));
    /**
     * @class PermissionProperty
     * 
     * Constructs this PermissionProperty object
     * function __construct() : void
     */
    permission_property_class->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });
    /**
     * @class PermissionProperty
     * Gets the name of this property
     * function getName() : string
     */
    permission_property_class->registerFunction("getName", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<PermissionPropertyObject> pp_obj = std::dynamic_pointer_cast<PermissionPropertyObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = pp_obj->name;
    });
    /**
     * @class PermissionProperty
     * Gets the value of this property
     * function getValue() : string
     */
    permission_property_class->registerFunction("getValue", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<PermissionPropertyObject> pp_obj = std::dynamic_pointer_cast<PermissionPropertyObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = pp_obj->value;       
    });
}

std::shared_ptr<Object> PermissionPropertyObject::newInstance(Class* c)
{
    return std::make_shared<PermissionPropertyObject>(c);
}