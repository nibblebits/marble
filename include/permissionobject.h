#ifndef PERMISSIONOBJECT_H
#define PERMISSIONOBJECT_H

#include "object.h"
class PermissionObject : public Object
{
public:
    PermissionObject(Class* c);
    virtual ~PermissionObject();

    virtual std::shared_ptr<Object> newInstance(Class* c);
};

#endif