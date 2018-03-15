#include "permissionpropertyobject.h"
#include "systemhandler.h"
PermissionPropertyObject::PermissionPropertyObject(Class* c) : Object(c)
{
    this->name = "";
    this->value = "";
}
PermissionPropertyObject::~PermissionPropertyObject()
{

}

Class* PermissionPropertyObject::registerClass(SystemHandler* systemHandler)
{
    Class* permission_property_class = systemHandler->getClassSystem()->registerClass("PermissionProperty");
    permission_property_class->setDescriptorObject(std::make_shared<PermissionPropertyObject>(permission_property_class));
    permission_property_class->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });
    permission_property_class->registerFunction("getName", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<PermissionPropertyObject> pp_obj = std::dynamic_pointer_cast<PermissionPropertyObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = pp_obj->name;
    });
    permission_property_class->registerFunction("getValue", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<PermissionPropertyObject> pp_obj = std::dynamic_pointer_cast<PermissionPropertyObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = pp_obj->value;       
    });
}

std::shared_ptr<Object> PermissionPropertyObject::newInstance(Class* c)
{
    return std::make_shared<PermissionPropertyObject>(c);
}