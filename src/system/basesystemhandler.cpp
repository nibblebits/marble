#include "basesystemhandler.h"
#include "exceptionobject.h"
#include "array.h"
BaseSystemHandler::BaseSystemHandler() : SystemHandler(SYSTEM_HANDLER_BASE_SYSTEM_HANDLER, NULL, NULL)
{

    // Register the Object class
    Class* c = getClassSystem()->registerClass("Object", NULL, CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER);
    c->registerFunction("toString", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = "toString() for object not implemented";
    });
    getClassSystem()->setDefaultBaseClass(c);

     /* Let's register an Exception class that is to be inherited by all classes that can be thrown*/
    Class* exception_class = getClassSystem()->registerClass("Exception");
    Variable msg_var;
    msg_var.type = VARIABLE_TYPE_STRING;
    msg_var.name = "message";
    msg_var.setValue("");
    exception_class->addVariable(msg_var);
    exception_class->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        object->getVariable("message")->setValue(arguments[0].svalue);
    });

    exception_class->registerFunction("getStackTrace", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> exception_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = exception_obj->getStackTrace();
    });
    // The Exception class has the Exception object instance.
    exception_class->setDescriptorObject(std::make_shared<ExceptionObject>(exception_class));
}

BaseSystemHandler::~BaseSystemHandler()
{

}
