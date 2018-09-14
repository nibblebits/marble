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