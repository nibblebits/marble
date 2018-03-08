#include "permissionobject.h"
#include <memory>
PermissionObject::PermissionObject(Class* c) : Object(c)
{

}

PermissionObject::~PermissionObject()
{
    
}

std::shared_ptr<Object> PermissionObject::newInstance(Class* c)
{
    return std::make_shared<PermissionObject>(c);
}