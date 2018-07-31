#include "requestargumentsobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"

WebModuleRequestArgumentsObject::WebModuleRequestArgumentsObject(Class* c) : Object(c)
{

}

WebModuleRequestArgumentsObject::~WebModuleRequestArgumentsObject()
{

}

std::shared_ptr<Object> WebModuleRequestArgumentsObject::newInstance(Class* c)
{
    return std::make_shared<WebModuleRequestArgumentsObject>(c);
}

Class* WebModuleRequestArgumentsObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("RequestArguments");
    c->setDescriptorObject(std::make_shared<WebModuleRequestArgumentsObject>(c));
    c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("boolean"), WebModuleRequestArgumentsObject::RequestArguments_has);
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("string"), WebModuleRequestArgumentsObject::RequestArguments_get);
}

void WebModuleRequestArgumentsObject::RequestArguments_has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleRequestArgumentsObject> args_obj = std::dynamic_pointer_cast<WebModuleRequestArgumentsObject>(object);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = (args_obj->arguments.find(values[0].svalue) != args_obj->arguments.end());
}

void WebModuleRequestArgumentsObject::RequestArguments_get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<WebModuleRequestArgumentsObject> args_obj = std::dynamic_pointer_cast<WebModuleRequestArgumentsObject>(object);
    return_value->type = VALUE_TYPE_STRING;

    if (args_obj->arguments.find(values[0].svalue) == args_obj->arguments.end())
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})));
    }

    return_value->svalue = args_obj->arguments[values[0].svalue];
}