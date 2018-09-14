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
     * Ensures we have cookie write access
     */
    static void ensureHeaderWriteAccess(Interpreter *interpreter, Scope *caller_scope);
    static Class *registerClass(ModuleSystem *moduleSystem);
};

#endif