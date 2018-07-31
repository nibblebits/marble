#ifndef WEBMODULE_REQUESTARGUMENTSOBJECT
#define WEBMODULE_REQUESTARGUMENTSOBJECT
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
class WebModuleRequestArgumentsObject : public Object
{
public:
    WebModuleRequestArgumentsObject(Class* c);
    virtual ~WebModuleRequestArgumentsObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    std::map<std::string, std::string> arguments;

    // Static Native RequestArgument functions
    static void RequestArguments_has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void RequestArguments_get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};


#endif