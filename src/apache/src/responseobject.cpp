#include "responseobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "modulesystem.h"
#include "cookiepermission.h"
#include "headerpermission.h"
#include "misc.h"
#include <iostream>

WebModule_ResponseObject::WebModule_ResponseObject(Class *c) : Object(c)
{
    this->req = NULL;
}

WebModule_ResponseObject::~WebModule_ResponseObject()
{
}

std::shared_ptr<Object> WebModule_ResponseObject::newInstance(Class *c)
{
    return std::make_shared<WebModule_ResponseObject>(c);
}

Class *WebModule_ResponseObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Response
     * 
     * Responsible for responding to the web client.
     * This class is used for setting headers and cookies
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Response");
    c->setDescriptorObject(std::make_shared<WebModule_ResponseObject>(c));

    /**
     * @class Response
     * 
     * Sets the cookie with the given name to the given value.
     * The clients web client is alerted of this request and will create the cookie on the local machine
     */
    c->registerFunction("setCookie", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), WebModule_ResponseObject::Response_setCookie);

    /**
     * @class Response
     * 
     * Sets a response header
     * function setHeader(string name, string value) : void
     */
    c->registerFunction("setHeader", {VarType::fromString("string"), VarType::fromString("string")}, {VarType::fromString("void")}, WebModule_ResponseObject::Response_setHeader);
}

void WebModule_ResponseObject::errorCheck(std::shared_ptr<WebModule_ResponseObject> obj)
{
    if (obj->req == NULL)
        throw std::logic_error("Request object has not been setup correctly");
}

void WebModule_ResponseObject::Response_setCookie(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<WebModule_ResponseObject> response_obj = std::dynamic_pointer_cast<WebModule_ResponseObject>(object);
    WebModule_ResponseObject::errorCheck(response_obj);

    // Ensure we have access to write cookies
    CookiePermission::ensureCookieWriteAccess(interpreter, caller_scope);

    std::string cookie_full = values[0].svalue + "=" + values[1].svalue;
    apr_table_set(response_obj->req->headers_out, "Set-Cookie", cookie_full.c_str());
}

void WebModule_ResponseObject::Response_setHeader(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<WebModule_ResponseObject> wms_obj = std::dynamic_pointer_cast<WebModule_ResponseObject>(object);
    WebModule_ResponseObject::errorCheck(wms_obj);

    // Ensure we have permission to set headers
    HeaderPermission::ensureHeaderWriteAccess(interpreter, caller_scope);

    if (to_lower(values[0].svalue) == "location")
    {
        // We need a 302 response code
        interpreter->properties["response_code"] = std::to_string(302);
    }
    else if(to_lower(values[0].svalue) == "set-cookie")
    {
        // We need the cookie permission to set cookies
        CookiePermission::ensureCookieWriteAccess(interpreter, caller_scope);
    }
      
    request_rec *req = wms_obj->req;
    apr_table_set(req->headers_out, values[0].svalue.c_str(), values[1].svalue.c_str());
}
