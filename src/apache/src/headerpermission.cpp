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

#include "headerpermission.h"
#include "modulesystem.h"
#include "function.h"
#include "interpreter.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "variable.h"

#include <string>

HeaderPermission::HeaderPermission(Class *c) : PermissionObject(c)
{
}

HeaderPermission::~HeaderPermission()
{
}

std::shared_ptr<Object> HeaderPermission::newInstance(Class* c)
{
    std::shared_ptr<HeaderPermission> header_permission = std::make_shared<HeaderPermission>(c);
    return header_permission;
}

void HeaderPermission::ensureHeaderWriteAccess(Interpreter *interpreter, Scope *caller_scope)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {

        std::shared_ptr<PermissionObject> permission_obj = caller_scope->getPermission("HeaderPermission");
        if (!permission_obj)
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You require a HeaderPermission to set HTTP headers and set response codes.", interpreter->getStackTraceLog());
    }
}

Class* HeaderPermission::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class HeaderPermission extends Permission
     * 
     * This permission must be held if you wish to create system headers
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("HeaderPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<HeaderPermission>(c));

    /**
     * @class HeaderPermission
     * 
     * constructs this header permission
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {

    });

    /*
    * @class HeaderPermission
    * 
    * This does nothing as HeaderPermission has no properties
    */
    c->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), Function::Blank);
}
