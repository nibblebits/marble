#include "filepermission.h"
#include "permissionpropertyobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include "interpreter.h"
#include "statics.h"
#include "misc.h"
#include <iostream>
FilePermission::FilePermission(Class* c) : PermissionObject(c)
{
    this->location = NULL;
    this->can_read = NULL;
    this->can_write = NULL;
}

FilePermission::~FilePermission()
{

}


std::shared_ptr<Object> FilePermission::newInstance(Class* c)
{
    std::shared_ptr<FilePermission> fperms = std::make_shared<FilePermission>(c);
    fperms->location = fperms->getVariableAnyScope("location");
    fperms->can_read = fperms->getVariableAnyScope("can_read");
    fperms->can_write = fperms->getVariableAnyScope("can_write");
    return fperms;
}

void FilePermission::registerClass(SystemHandler* systemHandler)
{
    Class* file_permission_cls = systemHandler->getClassSystem()->registerClass("FilePermission", systemHandler->getClassSystem()->getClassByName("Permission"));
    file_permission_cls->setDescriptorObject(std::make_shared<FilePermission>(file_permission_cls));

    // Create location variable
    Variable location;
    location.type = VARIABLE_TYPE_STRING;
    location.access = MODIFIER_ACCESS_PRIVATE;
    location.name = "location";
    location.setValue("/");
    file_permission_cls->addVariable(location);

    // Create the can_read variable
    Variable can_read;
    can_read.type = VARIABLE_TYPE_BOOLEAN;
    can_read.access = MODIFIER_ACCESS_PRIVATE;
    can_read.name = "can_read";
    can_read.setValue(false);
    file_permission_cls->addVariable(can_read);


    // Create the can_write variable
    Variable can_write;
    can_write.type = VARIABLE_TYPE_BOOLEAN;
    can_write.access = MODIFIER_ACCESS_PRIVATE;
    can_write.name = "can_write";
    can_write.setValue(false);
    file_permission_cls->addVariable(can_write);


    file_permission_cls->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);
    file_permission_cls->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), FilePermission::FilePermission_PermissionCheck);
    file_permission_cls->registerFunction("__prior_add", {}, VarType::fromString("void"), FilePermission::FilePermission_PriorAdd);

    /*
    * Sets the location this FilePermission should be bound to
    * function setLocation(string location) : void;
    */
    file_permission_cls->registerFunction("setLocation", {VarType::fromString("string")}, VarType::fromString("void"), FilePermission::FilePermission_SetLocation);

    /*
    * Gets the location that this FilePermission is bound to
    * function getLocation() : string;
    */
    file_permission_cls->registerFunction("getLocation", {}, VarType::fromString("string"), FilePermission::FilePermission_GetLocation);


    /*
     * Sets weather this FilePermission should allow reading from the location that this FilePermission is bound to
     * function setCanRead(boolean can_read) : void;
     */

    file_permission_cls->registerFunction("setCanRead", {VarType::fromString("boolean")}, VarType::fromString("void"), FilePermission::FilePermission_SetCanRead); 
    /*
    * Sets weather this FilePermission should allow writing to the location that this FilePermission is bound to
    * function setCanWrite(boolean can_write) : void;
    */
    file_permission_cls->registerFunction("setCanWrite", {VarType::fromString("boolean")}, VarType::fromString("void"), FilePermission::FilePermission_SetCanWrite);

}

void FilePermission::FilePermission_SetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    if (file_perm_obj->location->is_locked)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))));
    }
    file_perm_obj->location->setValue(getAbsolutePath(values[0].svalue));
    // Lock the variable so it cannot be changed again
    file_perm_obj->location->is_locked = true;
}

void FilePermission::FilePermission_GetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    return_value->set(file_perm_obj->location->value.svalue);
}

void FilePermission::FilePermission_SetCanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    if (file_perm_obj->can_read->is_locked)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))));
    }
    file_perm_obj->can_read->setValue((bool)values[0].dvalue);
    // Lock the variable so it cannot be changed again
    file_perm_obj->can_read->is_locked = true;
}

void FilePermission::FilePermission_SetCanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    if (file_perm_obj->can_write->is_locked)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))));
    }
    file_perm_obj->can_write->setValue((bool)values[0].dvalue);
    // Lock the variable so it cannot be changed again
    file_perm_obj->can_write->is_locked = true;
}

void FilePermission::FilePermission_CanRead(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    return_value->set((bool)file_perm_obj->can_read->value.dvalue);    
}

void FilePermission::FilePermission_CanWrite(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    return_value->set((bool)file_perm_obj->can_write->value.dvalue);    
}

void FilePermission::FilePermission_PermissionCheck(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    std::shared_ptr<PermissionPropertyObject> ours = std::dynamic_pointer_cast<PermissionPropertyObject>(values[0].ovalue);
    std::shared_ptr<PermissionPropertyObject> _new = std::dynamic_pointer_cast<PermissionPropertyObject>(values[1].ovalue);

    if (ours->name == "location")
    {
        std::string our_absolute_path = getAbsolutePath(ours->value);
        std::string _new_absolute_path = getAbsolutePath(_new->value);

        if (!startsWith(_new_absolute_path, our_absolute_path))
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))));
        }
    }
    else if("can_read")
    {
        bool ours_val = std::stoi(ours->value);
        bool new_val = std::stoi(_new->value);
        if (ours_val != new_val)
        {
            if (!ours_val && new_val)
            {
                // Attempting to create a permission with a true property while our own permission is false. This is illegal
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions");
            }
        }
    }
    else if("can_write")
    {
        bool ours_val = std::stoi(ours->value);
        bool new_val = std::stoi(_new->value);
        if (ours_val != new_val)
        {
            if (!ours_val && new_val)
            {
                // Attempting to create a permission with a true property while our own permission is false. This is illegal
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions");
            }
        }
    }

}


void FilePermission::FilePermission_PriorAdd(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    
}
