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