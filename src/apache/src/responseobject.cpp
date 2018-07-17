#include "responseobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "modulesystem.h"
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

    std::string cookie_full = values[0].svalue + "=" + values[1].svalue;
    apr_table_set(response_obj->req->headers_out, "Set-Cookie", cookie_full.c_str());
}
