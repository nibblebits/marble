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

#ifndef PERMISSIONOBJECT_H
#define PERMISSIONOBJECT_H

#include "object.h"
class Interpreter;
class PermissionObject : public Object
{
public:
    PermissionObject(Class* c);
    virtual ~PermissionObject();

    /**
     * Ensures that the provided PermissionObject is valid based on this PermissionObject.
     * The provided PermissionObject will have all its variables scanned and ensure the values match that of this PermissionObject
     * \param interpreter The interpreter that this check is for.
     * \param permission_obj The PermissionObject to validate
     * \param caller_scope The scope of the caller
     * \throws PermissionException Throws a PermissionException should validity prove false.
     * \throws std::logic_error Throws an std::logic_error should the class name of the provided object differ from our own.
     */
    void ensurePermissionValid(Interpreter* interpreter, std::shared_ptr<PermissionObject> permission_obj, Scope* caller_scope);
    static Class* registerClass(SystemHandler* systemHandler);
    virtual std::shared_ptr<Object> newInstance(Class* c);

};

#endif