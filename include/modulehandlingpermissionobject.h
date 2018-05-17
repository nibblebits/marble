#ifndef MODULEHANDLINGPERMISSIONOBJECT_H
#define MODULEHANDLINGPERMISSIONOBJECT_H

#include "permissionobject.h"

/**
 * This is the permission used for handling modules.
 * Without it no programs written in marble can load modules
 */
class ModuleHandlingPermissionObject : public PermissionObject
{
public:
    ModuleHandlingPermissionObject(Class* c);
    virtual ~ModuleHandlingPermissionObject();
    static Class* registerClass(SystemHandler* systemHandler);
    virtual std::shared_ptr<Object> newInstance(Class* c);

};

#endif