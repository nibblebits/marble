#include "permissionobject.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"
#include "systemhandler.h"
#include "interpreter.h"
#include "function.h"
#include "value.h"
#include <memory>
#include <iostream>
PermissionObject::PermissionObject(Class* c) : Object(c)
{

}

PermissionObject::~PermissionObject()
{
    
}

Class* PermissionObject::registerClass(SystemHandler* systemHandler)
{
    Class* permission_class = systemHandler->getClassSystem()->registerClass("Permission");
    permission_class->setDescriptorObject(std::make_shared<PermissionObject>(permission_class));
    permission_class->is_pure = true;
    permission_class->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });
    /*
     * This function __permission_check is pure and called when the system cannot ensure weather a permission is legal or not.
     * Simply put when adding new permissions your own permissions are checked to see if you are allowed to do it.
     * However permission variables must also be ensured and if your permission variables are not equal to the permission you are trying to add
     * then this pure function will be called so that the permission can decide weather or not to allow this action.
     * The first PermissionProperty argument is the PermissionProperty you are trying to set and the second is the PermissionProperty of the permission you do not yet hold.*/ 
    Function* p_check_func = permission_class->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
    });

    /**
     * This function __prior_add is a function that is called just before this PermissionObject is about to be added to a PermissionsObject.
     * In this method you should ensure that the user has set all the required attributes and that they are locked so that
     * they cannot be changed. This is not a pure method so you must override it to use its functionality.
     * 
     * This function is also only called when calling the add method on the PermissionObject. Not the native functions provided.
     * 
     * function __prior_add() : void
     */

    Function* prior_add_func = permission_class->registerFunction("__prior_add", {}, VarType::fromString("void"), Function::Blank);
    p_check_func->is_pure = true;
    return permission_class;
}

void PermissionObject::ensurePermissionValid(Interpreter* interpreter, std::shared_ptr<PermissionObject> permission_obj, Scope* caller_scope)
{
    if (permission_obj->getClass()->name != this->getClass()->name)
        throw std::logic_error("Passing a PermissionObject whose class differs from this objects class");

    std::vector<Variable*> variables = this->getVariables();
    for (Variable* var : variables)
    {
        Variable* other_permission_var = permission_obj->getVariableAnyScope(var->name);
        // Ignore this and super we don't want to use them for this permission checking as they are unrelated to the rules
        if (other_permission_var->name == "this" 
                || other_permission_var->name == "super")
            continue;
        if (other_permission_var->value != var->value)
        {
            // The permission values do not match so we must invoke our own __permission_check function to allow or disallow this action
            // The function should throw an exception should this be rejected
            std::shared_ptr<PermissionPropertyObject> f_property = std::dynamic_pointer_cast<PermissionPropertyObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionProperty")));
            std::shared_ptr<PermissionPropertyObject> s_property = std::dynamic_pointer_cast<PermissionPropertyObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionProperty")));
            
            f_property->name = var->name;
            f_property->value = Value::getStringValue(&var->value);
            s_property->name = var->name;
            s_property->value = Value::getStringValue(&other_permission_var->value);

            Value f_property_value;
            Value s_property_value;
            Value return_value;
            f_property_value.set(f_property);
            s_property_value.set(s_property);


            // Now that we have these property variables setup we can now get and invoke the __permission_check function
            Function* f = permission_obj->getClass()->getFunctionByNameAndArguments("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")});
            f->invoke(interpreter, {f_property_value, s_property_value}, &return_value, shared_from_this(), caller_scope);
        }
    }
}
std::shared_ptr<Object> PermissionObject::newInstance(Class* c)
{
    return std::make_shared<PermissionObject>(c);
}
