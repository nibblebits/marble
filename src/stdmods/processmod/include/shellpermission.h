#ifndef SHELLPERMISSION_H
#define SHELLPERMISSION_H

#include "permissionobject.h"

/**
 * The ShellPermission is required by anyone who wants to execute shell commands
 */
class ModuleSystem;
class Interpreter;
class ShellPermission : public PermissionObject
{
    public:
        ShellPermission(Class* c);
        virtual ~ShellPermission();
        virtual std::shared_ptr<Object> newInstance(Class* c);
        /**
         * Ensures that the calling scope has the network permission
         */
        static void ensurePermission(Interpreter* interpreter, Scope* caller_scope);
        static void registerClass(ModuleSystem* moduleSystem);
};

#endif