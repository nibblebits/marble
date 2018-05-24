#include "webmod.h"
#include "apache_mod.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "misc.h"

WebModuleRequestArgumentsObject::WebModuleRequestArgumentsObject(Class* c) : Object(c)
{

}

WebModuleRequestArgumentsObject::~WebModuleRequestArgumentsObject()
{

}

WebModulePOSTContentObject::WebModulePOSTContentObject(Class* c) : Object(c)
{
    
}

WebModulePOSTContentObject::~WebModulePOSTContentObject()
{

}


WebModuleObject::WebModuleObject(Class* c) : Object(c)
{

}

WebModuleObject::~WebModuleObject()
{

}



WebModule::WebModule() : Module("webmod", "Web Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

WebModule::~WebModule()
{

}

void WebModule::Init()
{
    log("Web Module initialised", LOG_LEVEL_NOTICE);

    /* RequestArguments Class*/
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("RequestArguments");
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleRequestArgumentsObject> args_obj = std::dynamic_pointer_cast<WebModuleRequestArgumentsObject>(object);
        return_value->type = VALUE_TYPE_STRING;

        if (args_obj->arguments.find(arguments[0].svalue) == args_obj->arguments.end())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})));
        }

        return_value->svalue = args_obj->arguments[arguments[0].svalue];
    });

    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleRequestArgumentsObject> args_obj = std::dynamic_pointer_cast<WebModuleRequestArgumentsObject>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = (args_obj->arguments.find(arguments[0].svalue) != args_obj->arguments.end());
    });
    /* End of RequestArguments Class */


    /* POSTContent class */
    c = this->getModuleSystem()->getClassSystem()->registerClass("POSTContent");
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModulePOSTContentObject> post_obj = std::dynamic_pointer_cast<WebModulePOSTContentObject>(object);
        return_value->type = VALUE_TYPE_STRING;

        if (post_obj->content.find(arguments[0].svalue) == post_obj->content.end())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})));
        }

        return_value->svalue = post_obj->content[arguments[0].svalue];
    });

    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModulePOSTContentObject> post_obj = std::dynamic_pointer_cast<WebModulePOSTContentObject>(object);
        return_value->type = VALUE_TYPE_NUMBER;
        return_value->dvalue = (post_obj->content.find(arguments[0].svalue) != post_obj->content.end());
    });
    /* End of POSTContent class */

    /* Request class */
    c = this->getModuleSystem()->getClassSystem()->registerClass("Request");
    c->registerFunction("getUri", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->request_uri;
    });

    c->registerFunction("getMethod", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->request_method;
    });

    c->registerFunction("getArguments", {}, VarType::fromString("RequestArguments"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = web_mod_obj->request_arguments;
    });

    c->registerFunction("getContent", {}, VarType::fromString("POSTContent"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_OBJECT;
        return_value->ovalue = web_mod_obj->content;
    });

    c->registerFunction("getRequesterIP", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<WebModuleObject> web_mod_obj = std::dynamic_pointer_cast<WebModuleObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = web_mod_obj->requester_ip;
    });

    /* End of Request class */

}

void WebModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to Request. 
    Scope* root_scope = interpreter->getRootScope();
    std::shared_ptr<WebModuleObject> object = std::make_shared<WebModuleObject>(this->getModuleSystem()->getClassSystem()->getClassByName("Request"));
    object->request_arguments = std::make_shared<WebModuleRequestArgumentsObject>(this->getModuleSystem()->getClassSystem()->getClassByName("RequestArguments"));
    object->content = std::make_shared<WebModulePOSTContentObject>(this->getModuleSystem()->getClassSystem()->getClassByName("POSTContent"));
    root_scope->createVariable("Request", "Request", object);
}

void WebModule::parseRequest(Interpreter* interpreter, request_rec* req)
{
    Scope* root_scope = interpreter->getRootScope();
    Variable* variable = root_scope->getVariableAnyScope("Request");

    std::shared_ptr<WebModuleObject> object = std::dynamic_pointer_cast<WebModuleObject>(variable->value.ovalue);
    object->request_uri = req->unparsed_uri;
    object->requester_ip = req->useragent_ip;
    object->request_method = req->method;
    object->request_arguments->arguments = parseGet(req);
    object->content->content = parsePost(req);
}

std::map<std::string, std::string> WebModule::parsePost(request_rec* req)
{
    std::map<std::string, std::string> m;
    keyValuePair* kvp = readPost(req);
    if (kvp == NULL)
        return m;

     for (int i = 0; &kvp[i]; i++) {
        if (kvp[i].key) {
            m[kvp[i].key] = (kvp[i].value ? kvp[i].value : "");
        }
        else
            break;
     }

     return m;
}

std::map<std::string, std::string> WebModule::parseGet(request_rec* req)
{
    std::map<std::string, std::string> m;

    if (req->parsed_uri.query == NULL)
        return m;
    m["query"] = req->parsed_uri.query;

    std::string query_str = req->parsed_uri.query;
    std::string name = "";
    std::string value = "";
    bool do_name = true;
    for (int i = 0; i < query_str.size(); i++)
    {
        char c = query_str[i];
        if (c == '=')
        {
            do_name = false;
        }
        else if(c == '&')
        {
            m[urlDecode(name)] = urlDecode(value);
            name = "";
            value = "";
            do_name = true;
        }
        else
        {
            if (do_name)
                name += c;
            else
                value += c;
        }
    }

    if (name != "")
        m[urlDecode(name)] = urlDecode(value);
    
    return m;
}