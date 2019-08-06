/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef NETWORKPERMISSION_H
#define NETWORKPERMISSION_H

#include "permissionobject.h"

enum
{
    NETWORK_PERMISSION_EMAIL_PERMISSION_REQUIRED,
    NETWORK_PERMISSION_CURL_PERMISSION_REQUIRED,
    NETWORK_PERMISSION_LOOKUP_PERMISSION_REQUIRED,
    NETWORK_PERMISSION_SOCKET_PERMISSION_REQUIRED,
    NETWORK_PERMISSION_SOCKET_SERVER_PERMISSION_REQUIRED
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
        Variable* can_socket;
        Variable* can_socket_server;

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
        static void NetworkPermission_setCanSocket(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_setCanSocketServer(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);

        static void NetworkPermission_getCanSendEmail(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanUseCurl(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanLookup(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanSocket(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);
        static void NetworkPermission_getCanSocketServer(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);

};

#endif