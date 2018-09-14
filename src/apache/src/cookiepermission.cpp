#include "cookiepermission.h"
#include "modulesystem.h"
#include "function.h"
#include "interpreter.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "variable.h"
#include <string>

CookiePermission::CookiePermission(Class* c) : PermissionObject(c)
{
    this->can_read = NULL;
    this->can_write = NULL;
}

CookiePermission::~CookiePermission()
{

}

std::shared_ptr<Object> CookiePermission::newInstance(Class* c)
{
    std::shared_ptr<CookiePermission> cookie_permission =  std::make_shared<CookiePermission>(c);
    cookie_permission->can_read = cookie_permission->getVariableAnyScope("can_read");
    cookie_permission->can_write = cookie_permission->getVariableAnyScope("can_write");
    return cookie_permission;
}


Class* CookiePermission::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("CookiePermission", moduleSystem->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<CookiePermission>(c));

    // Setup the can_read variable
    Variable can_read;
    can_read.type = VARIABLE_TYPE_BOOLEAN;
    can_read.access = MODIFIER_ACCESS_PRIVATE;
    can_read.name = "can_read";
    can_read.setValue(false);

    // Setup the can_write variable
    Variable can_write;
    can_write.type = VARIABLE_TYPE_BOOLEAN;
    can_write.access = MODIFIER_ACCESS_PRIVATE;
    can_write.name = "can_write";
    can_write.setValue(false);

    // Add our variables to the class
    c->addVariable(can_read);
    c->addVariable(can_write);

    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
            
    });

    /**
     * 
     * Sets this permission's read access
     * 
     * function setCanRead(boolean can_read) : void
     */
    c->registerFunction("setCanRead", {VarType::fromString("boolean")}, VarType::fromString("void"), CookiePermission::CookiePermission_setCanRead);

    /**
     * 
     * Sets this permission's write access
     * 
     * function setCanWrite(boolean can_write) : void
     */
    c->registerFunction("setCanWrite", {VarType::fromString("boolean")}, VarType::fromString("void"), CookiePermission::CookiePermission_setCanWrite);



    // We need to override this pure function but we don't plan to do anything with it..
    c->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), CookiePermission::CookiePermission__permission_check);

}

void CookiePermission::CookiePermission_setCanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CookiePermission> cookie_permission_obj = std::dynamic_pointer_cast<CookiePermission>(object);
    cookie_permission_obj->can_write->setValue(values[0].dvalue);
}


void CookiePermission::CookiePermission_setCanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CookiePermission> cookie_permission_obj = std::dynamic_pointer_cast<CookiePermission>(object);
    cookie_permission_obj->can_read->setValue(values[0].dvalue);
}

void CookiePermission::CookiePermission__permission_check(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<PermissionPropertyObject> our_property = std::dynamic_pointer_cast<PermissionPropertyObject>(values[0].ovalue);
    std::shared_ptr<PermissionPropertyObject> their_property = std::dynamic_pointer_cast<PermissionPropertyObject>(values[1].ovalue);
    
    if (our_property->name == "can_write")
    {
        bool ours_val = std::stoi(our_property->value);
        bool new_val = std::stoi(their_property->value);

        if (!ours_val && new_val)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions of a CookiePermission past your own permissions", interpreter->getStackTraceLog());
        }
    }
    else if (our_property->name == "can_read")
    {
        bool ours_val = std::stoi(our_property->value);
        bool new_val = std::stoi(their_property->value);

        if (!ours_val && new_val)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions of a CookiePermission past your own permissions", interpreter->getStackTraceLog());
        }
    }
}