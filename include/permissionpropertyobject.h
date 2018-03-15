#ifndef PERMISSIONPROPERTYOBJECT_H
#define PERMISSIONPROPERTYOBJECT_H

#include "object.h"
class PermissionPropertyObject : public Object
{
public:
    PermissionPropertyObject(Class* c);
    virtual ~PermissionPropertyObject();

    static Class* registerClass(SystemHandler* systemHandler);
    virtual std::shared_ptr<Object> newInstance(Class* c);

    /**
     * The name of the permission property
     */
    std::string name;
    /**
     * The value of the permission property
     */
    std::string value;
};

#endif