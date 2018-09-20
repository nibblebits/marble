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

#ifndef HEADERPERMISSION_H
#define HEADERPERMISSION_H

#include "permissionobject.h"
#include <string>

class ModuleSystem;
/**
 * This permission is held by all those wanting to set headers
 */
class HeaderPermission : public PermissionObject
{
  public:
    HeaderPermission(Class *c);
    virtual ~HeaderPermission();
    virtual std::shared_ptr<Object> newInstance(Class *c);

    /**
     * Ensures we have header write access
     */
    static void ensureHeaderWriteAccess(Interpreter *interpreter, Scope *caller_scope);

    /**
     * Ensures we have header write access
     */
    static void ensureHeaderReadAccess(Interpreter *interpreter, Scope *caller_scope);

    static Class *registerClass(ModuleSystem *moduleSystem);
};

#endif