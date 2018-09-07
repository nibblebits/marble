#ifndef NETWORKPERMISSION_H
#define NETWORKPERMISSION_H

#include "permissionobject.h"

enum
{
    NETWORK_PERMISSION_EMAIL_PERMISSION_REQUIRED,
    NETWORK_PERMISSION_CURL_PERMISSION_REQUIRED,
    NETWORK_PERMISSION_LOOKUP_PERMISSION_REQUIRED
};

typedef int NETWORK_PERMISSION_TYPE;

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

        /* Permissions */
        Variable* can_send_email;
        Variable* can_use_curl;
        Variable* can_lookup;

        /**
         * Ensures that the calling scope has the network permission
         */
        static void ensurePermission(Interpreter* interpreter, Scope* caller_scope, NETWORK_PERMISSION_TYPE perm_type);
        static void registerClass(ModuleSystem* moduleSystem);


        // Native NetworkPermission methods
        static void NetworkPermission_PermissionCheck(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_setCanSendEmail(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_setCanUseCurl(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_setCanLookup(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanSendEmail(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanUseCurl(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanLookup(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);

};

#endif