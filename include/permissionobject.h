#ifndef PERMISSIONOBJECT_H
#define PERMISSIONOBJECT_H

#include "object.h"
class PermissionObject : public Object
{
public:
    PermissionObject(Class* c);
    virtual ~PermissionObject();

    static Class* registerClass(SystemHandler* systemHandler);
    virtual std::shared_ptr<Object> newInstance(Class* c);
};

#endif