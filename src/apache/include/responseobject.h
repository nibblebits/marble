#ifndef WEBMODULE_RESPONSEOBJECT
#define WEBMODULE_RESPONSEOBJECT
#include "object.h"
#include "httpd.h"
#include "apache_mod.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"
#include "module.h"
#include "class.h"
#include <stdio.h>
#include <string>
#include <memory>
class WebModule_ResponseObject : public Object
{
public:
    WebModule_ResponseObject(Class* c);
    virtual ~WebModule_ResponseObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    /**
     * Checks to see if the WebModule_ResponseObject is free from errors
     * throws an exception if there are problems
     */
    static void errorCheck(std::shared_ptr<WebModule_ResponseObject> obj);

    // Native ResponseObject Marble Methods
    static void Response_setCookie(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    // The given apache request object
    request_rec* req;

};


#endif