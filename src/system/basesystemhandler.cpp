#include "basesystemhandler.h"
#include "exceptionobject.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"
#include "modulehandlingpermissionobject.h"
#include "array.h"
#include "function.h"
BaseSystemHandler::BaseSystemHandler() : SystemHandler(SYSTEM_HANDLER_BASE_SYSTEM_HANDLER, NULL, NULL)
{

    // Register the Object class
    Class* c = getClassSystem()->registerClass("Object", NULL, CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER);
    c->registerFunction("__construct", {}, VarType::fromString("string"), Function::Blank);
    c->registerFunction("toString", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = "toString() for object not implemented";
    });
    getClassSystem()->setDefaultBaseClass(c);

     /* Let's register an Exception class that is to be inherited by all classes that can be thrown*/
    Class* exception_class = getClassSystem()->registerClass("Exception");
    exception_class->registerFunction("__construct", { }, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        // Blank constructor so just call the parent
        object->getClass()->invokeObjectParentConstructor({}, object, interpreter);
    });
    exception_class->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> except_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        except_obj->setMessage(arguments[0].svalue);
    });

    exception_class->registerFunction("getMessage", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> except_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        return_value->set(except_obj->getMessage());
    });
    
    exception_class->registerFunction("getStackTrace", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> exception_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = exception_obj->getStackTrace();
    });
    // The Exception class has the Exception object instance.
    exception_class->setDescriptorObject(std::make_shared<ExceptionObject>(exception_class));

    // Create a permissions exception
    c = getClassSystem()->registerClass("PermissionException", exception_class);
        c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });



    // We need a permission class to help manage permissions
    PermissionObject::registerClass(this);

    // We also need a permissions class that will hold the permissions for a current scope
    PermissionsObject::registerClass(this);

    // We need to register the PermissionProperty class which will hold a variable name and value for a permission variable.
    PermissionPropertyObject::registerClass(this);

    // We need a module handling permissions object that will allow a marble programmer to load modules
    ModuleHandlingPermissionObject::registerClass(this);
}

BaseSystemHandler::~BaseSystemHandler()
{

}
