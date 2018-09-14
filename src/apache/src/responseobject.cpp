#include "responseobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "modulesystem.h"
#include "cookiepermission.h"
#include <iostream>

WebModule_ResponseObject::WebModule_ResponseObject(Class* c) : Object(c)
{
    this->req = NULL;
}

WebModule_ResponseObject::~WebModule_ResponseObject()
{

}

std::shared_ptr<Object> WebModule_ResponseObject::newInstance(Class* c)
{
    return std::make_shared<WebModule_ResponseObject>(c);
}

Class* WebModule_ResponseObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("Response");
    c->setDescriptorObject(std::make_shared<WebModule_ResponseObject>(c));

    /**
     * Sets the cookie with the given name to the given value.
     * The clients web client is alerted of this request and will create the cookie on the local machine
     */
    c->registerFunction("setCookie", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), WebModule_ResponseObject::Response_setCookie);
}

void WebModule_ResponseObject::errorCheck(std::shared_ptr<WebModule_ResponseObject> obj)
{
  if (obj->req == NULL)
    throw std::logic_error("Request object has not been setup correctly");
}

void WebModule_ResponseObject::Response_setCookie(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModule_ResponseObject> response_obj = std::dynamic_pointer_cast<WebModule_ResponseObject>(object);
    WebModule_ResponseObject::errorCheck(response_obj);


    if (!interpreter->hasNoPermissionRestrictions())
    {

        std::vector<std::shared_ptr<PermissionObject>> permission_list = caller_scope->getPermissionList("CookiePermission");
        // If the permission list is empty then we don't have permission to write cookies
        if (permission_list.empty())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the CookiePermission which is required for reading and writing cookies", interpreter->getStackTraceLog());
        }

        bool has_access = false;
        for (std::shared_ptr<PermissionObject> permission_obj : permission_list)
        {
            std::shared_ptr<CookiePermission> permission = std::dynamic_pointer_cast<CookiePermission>(permission_obj);
            
            if (permission->can_write->value.dvalue)
            {
                has_access = true;
                break;
            }
        }

        if (!has_access)
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the CookiePermission write access which is required for writing cookies", interpreter->getStackTraceLog());
    }

    std::string cookie_full = values[0].svalue + "=" + values[1].svalue;
    apr_table_set(response_obj->req->headers_out, "Set-Cookie", cookie_full.c_str());
}
