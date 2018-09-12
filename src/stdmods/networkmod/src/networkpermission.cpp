#include "networkpermission.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "statics.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"

NetworkPermission::NetworkPermission(Class *c) : PermissionObject(c)
{
    this->can_send_email = NULL;
    this->can_use_curl = NULL;
    this->can_lookup = NULL;
}

NetworkPermission::~NetworkPermission()
{
}

std::shared_ptr<Object> NetworkPermission::newInstance(Class *c)
{
    std::shared_ptr<NetworkPermission> nperms = std::make_shared<NetworkPermission>(c);
    nperms->can_send_email = nperms->getVariableAnyScope("can_send_email");
    nperms->can_use_curl = nperms->getVariableAnyScope("can_send_email");
    nperms->can_lookup = nperms->getVariableAnyScope("can_lookup");
    return nperms;
}

void NetworkPermission::ensurePermission(Interpreter *interpreter, Scope *caller_scope, NETWORK_PERMISSION_TYPE perm_type)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {
        std::vector<std::shared_ptr<PermissionObject>> permission_list = caller_scope->getPermissionList("NetworkPermission");
        // If the permission list is empty then we don't have permission to open this file
        if (permission_list.empty())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the NetworkPermission which is required for network operations");
        }

        bool hasPermission = false;
        std::string fail_msg = "";
        if (perm_type == NETWORK_PERMISSION_EMAIL_PERMISSION_REQUIRED)
            fail_msg = "You do not have the send email permission for the NetworkPermission to send emails. Seek setCanSendEmail()";
        else if (perm_type == NETWORK_PERMISSION_CURL_PERMISSION_REQUIRED)
            fail_msg = "You do not have the curl permission for the NetworkPermission to use curl. Seek setCanUseCurl()";
        else if (perm_type == NETWORK_PERMISSION_LOOKUP_PERMISSION_REQUIRED)
            fail_msg = "You must have the lookup permission for the NetworkPermission for you to do lookups of any kind such as hostname lookups. Seek setCanLookup()";

        for (std::shared_ptr<PermissionObject> perm : permission_list)
        {
            std::shared_ptr<NetworkPermission> permission = std::dynamic_pointer_cast<NetworkPermission>(perm);
            if (perm_type == NETWORK_PERMISSION_EMAIL_PERMISSION_REQUIRED && permission->can_send_email->value.dvalue)
            {
                hasPermission = true;
                break;
            }
            else if (perm_type == NETWORK_PERMISSION_CURL_PERMISSION_REQUIRED && permission->can_use_curl->value.dvalue)
            {
                hasPermission = true;
                break;
            }
            else if (perm_type == NETWORK_PERMISSION_LOOKUP_PERMISSION_REQUIRED && permission->can_lookup->value.dvalue)
            {
                hasPermission = true;
                break;
            }
        }

        if (!hasPermission)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), fail_msg);
        }
    }
}

void NetworkPermission::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class NetworkPermission extends Permission
     * 
     * The NetworkPermission must be held by all those who wish to preform network operations.
     * The type of operation determines what attributes must be set within this NetworkPermission
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("NetworkPermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<NetworkPermission>(c));

    // Create can_send_email variable
    Variable can_send_email;
    can_send_email.type = VARIABLE_TYPE_BOOLEAN;
    can_send_email.access = MODIFIER_ACCESS_PRIVATE;
    can_send_email.name = "can_send_email";
    can_send_email.setValue(false);
    c->addVariable(can_send_email);

    // Create can_use_curl variable
    Variable can_use_curl;
    can_use_curl.type = VARIABLE_TYPE_BOOLEAN;
    can_use_curl.access = MODIFIER_ACCESS_PRIVATE;
    can_use_curl.name = "can_use_curl";
    can_use_curl.setValue(false);
    c->addVariable(can_use_curl);

    // Create can_lookup variable
    Variable can_lookup;
    can_lookup.type = VARIABLE_TYPE_BOOLEAN;
    can_lookup.access = MODIFIER_ACCESS_PRIVATE;
    can_lookup.name = "can_lookup";
    can_lookup.setValue(false);
    c->addVariable(can_lookup);

    /**
     * @class NetworkPermission
     * 
     * Constructs this NetworkPermission
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);
    /**
     * @class NetworkPermission
     * 
     * This function is called when adding a NetworkPermission to another Permissions object to ensure that you are allowed to do it
     * function __permission_check(PermissionProperty p1, PermissionProperty p2) : void
     */
    c->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), NetworkPermission::NetworkPermission_PermissionCheck);

    /**
     * @class NetworkPermission
     * 
     * This function sets this Network Permission's canSendEmail property allowing the holder of this NetworkPermission to send emails
     * function setCanSendEmail(boolean can_send_email) : void
     */
    c->registerFunction("setCanSendEmail", {VarType::fromString("boolean")}, VarType::fromString("void"), NetworkPermission::NetworkPermission_setCanSendEmail);

    /**
     * @class NetworkPermission
     * 
     * This function sets this Network Permission's canUseCurl property allowing the holder of this NetworkPermission to use curl
     * function setCanUseCurl(boolean can_use_curl) : void
     */
    c->registerFunction("setCanUseCurl", {VarType::fromString("boolean")}, VarType::fromString("void"), NetworkPermission::NetworkPermission_setCanUseCurl);

 /**
     * @class NetworkPermission
     * 
     * This function sets this Network Permission's canLookup property allowing the holder of this NetworkPermission to lookup hostnames and other Network related information
     * function setCanLookup(boolean can_lookup) : void
     */
    c->registerFunction("setCanLookup", {VarType::fromString("boolean")}, VarType::fromString("void"), NetworkPermission::NetworkPermission_setCanLookup);

    /**
     * @class NetworkPermission
     * 
     * This function returns true if this NetworkPermission allows us to send emails
     * function getCanSendEmail() : boolean
     */
    c->registerFunction("getCanSendEmail", {}, VarType::fromString("boolean"), NetworkPermission::NetworkPermission_getCanSendEmail);

    /**
     * @class NetworkPermission
     * 
     * This function returns true if this NetworkPermission allows access to curl
     * function getCanUseCurl() : boolean
     */
    c->registerFunction("getCanUseCurl", {}, VarType::fromString("boolean"), NetworkPermission::NetworkPermission_getCanUseCurl);

    /**
     * @class NetworkPermission
     * 
     * This function returns true if this NetworkPermission allows us to lookup hostnames and other network information
     * function getCanLookup() : boolean
     */
    c->registerFunction("getCanLookup", {}, VarType::fromString("boolean"), NetworkPermission::NetworkPermission_getCanLookup);

}

void NetworkPermission::NetworkPermission_PermissionCheck(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    std::shared_ptr<PermissionPropertyObject> ours = std::dynamic_pointer_cast<PermissionPropertyObject>(values[0].ovalue);
    std::shared_ptr<PermissionPropertyObject> _new = std::dynamic_pointer_cast<PermissionPropertyObject>(values[1].ovalue);

    if (ours->name == "can_send_email")
    {
        bool ours_val = std::stoi(ours->value);
        bool new_val = std::stoi(_new->value);
        if (ours_val != new_val)
        {
            if (!ours_val && new_val)
            {
                // Attempting to create a permission with a true property while our own permission is false. This is illegal
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions. For the NetworkPermission");
            }
        }
    }
    else if (ours->name == "can_use_curl")
    {
        bool ours_val = std::stoi(ours->value);
        bool new_val = std::stoi(_new->value);
        if (ours_val != new_val)
        {
            if (!ours_val && new_val)
            {
                // Attempting to create a permission with a true property while our own permission is false. This is illegal
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions. For the NetworkPermission");
            }
        }
    }
    else if (ours->name == "can_lookup")
    {
        bool ours_val = std::stoi(ours->value);
        bool new_val = std::stoi(_new->value);
        if (ours_val != new_val)
        {
            if (!ours_val && new_val)
            {
                // Attempting to create a permission with a true property while our own permission is false. This is illegal
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions. For the NetworkPermission");
            }
        }
    }
}

void NetworkPermission::NetworkPermission_setCanSendEmail(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    network_perm_obj->can_send_email->setValue(values[0]);
}

void NetworkPermission::NetworkPermission_setCanUseCurl(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    network_perm_obj->can_use_curl->setValue(values[0]);
}

void NetworkPermission::NetworkPermission_setCanLookup(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    network_perm_obj->can_lookup->setValue(values[0]);
}



void NetworkPermission::NetworkPermission_getCanSendEmail(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    return_value->set(&network_perm_obj->can_send_email->value);
}

void NetworkPermission::NetworkPermission_getCanUseCurl(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    return_value->set(&network_perm_obj->can_use_curl->value);
}

void NetworkPermission::NetworkPermission_getCanLookup(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<NetworkPermission> network_perm_obj = std::dynamic_pointer_cast<NetworkPermission>(object);
    return_value->set(&network_perm_obj->can_lookup->value);
}