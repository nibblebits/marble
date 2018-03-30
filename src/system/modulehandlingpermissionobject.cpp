#include "modulehandlingpermissionobject.h"
#include "systemhandler.h"
ModuleHandlingPermissionObject::ModuleHandlingPermissionObject(Class* c) : PermissionObject(c)
{

}
ModuleHandlingPermissionObject::~ModuleHandlingPermissionObject()
{

}
Class* ModuleHandlingPermissionObject::registerClass(SystemHandler* systemHandler)
{
    Class* c = systemHandler->getClassSystem()->registerClass("ModuleHandlingPermission", systemHandler->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<ModuleHandlingPermissionObject>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        
    });
}
std::shared_ptr<Object> ModuleHandlingPermissionObject::newInstance(Class* c)
{
    return std::make_shared<ModuleHandlingPermissionObject>(c);
}