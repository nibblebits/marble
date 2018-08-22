#ifndef NETWORKPERMISSION_H
#define NETWORKPERMISSION_H

#include "permissionobject.h"

/**
 * The NetworkPermission is required by anyone doing network operations
 */
class ModuleSystem;
class Interpreter;
class NetworkPermission : public PermissionObject
{
    public:
        NetworkPermission(Class* c);
        virtual ~NetworkPermission();
        virtual std::shared_ptr<Object> newInstance(Class* c);
        /**
         * Ensures that the calling scope has the network permission
         */
        static void ensurePermission(Interpreter* interpreter, Scope* caller_scope);
        static void registerClass(ModuleSystem* moduleSystem);
};

#endif