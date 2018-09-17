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

#ifndef COOKIEPERMISSION_H
#define COOKIEPERMISSION_H

#include "permissionobject.h"
#include <string>

class ModuleSystem;
class CookiePermission : public PermissionObject
{
public:
   CookiePermission(Class* c);
   virtual ~CookiePermission();
   virtual std::shared_ptr<Object> newInstance(Class* c);
 
   static Class* registerClass(ModuleSystem* moduleSystem);

    /**
     * Ensures the given scope has a cookie write access permission
     */
   static void ensureCookieWriteAccess(Interpreter* interpreter, Scope* caller_scope);
   
   // If true the holder of this permission can read cookies
   Variable* can_read;

   // If true the holder of this permission can write cookies
   Variable* can_write;

   // Native CookiePermission Marble Methods
   static void CookiePermission_setCanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
   static void CookiePermission_setCanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
   static void CookiePermission__permission_check(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};

#endif