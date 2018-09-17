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

#include "modulehandlingpermissionobject.h"
#include "systemhandler.h"
ModuleHandlingPermissionObject::ModuleHandlingPermissionObject(Class* c) : PermissionObject(c)
{

}
ModuleHandlingPermissionObject::~ModuleHandlingPermissionObject()
{

}
Class* ModuleHandlingPermissionObject::registerClass(SystemHandler* systemHandler)
{
    Class* c = systemHandler->getClassSystem()->registerClass("ModuleHandlingPermission", systemHandler->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<ModuleHandlingPermissionObject>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        
    });
}
std::shared_ptr<Object> ModuleHandlingPermissionObject::newInstance(Class* c)
{
    return std::make_shared<ModuleHandlingPermissionObject>(c);
}