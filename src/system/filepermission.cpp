/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
    * @class FilePermission
    * Sets the location this FilePermission should be bound to
    * function setLocation(string location) : void;
    */
    file_permission_cls->registerFunction("setLocation", {VarType::fromString("string")}, VarType::fromString("void"), FilePermission::FilePermission_SetLocation);

    /*
    * @class FilePermission
    * Gets the location that this FilePermission is bound to
    * function getLocation() : string;
    */
    file_permission_cls->registerFunction("getLocation", {}, VarType::fromString("string"), FilePermission::FilePermission_GetLocation);


    /*
     * @class FilePermission
     * Sets weather this FilePermission should allow reading from the location that this FilePermission is bound to
     * function setCanRead(boolean can_read) : void;
     */

    file_permission_cls->registerFunction("setCanRead", {VarType::fromString("boolean")}, VarType::fromString("void"), FilePermission::FilePermission_SetCanRead); 
    /*
    * @class FilePermission
    * Sets weather this FilePermission should allow writing to the location that this FilePermission is bound to
    * function setCanWrite(boolean can_write) : void;
    */
    file_permission_cls->registerFunction("setCanWrite", {VarType::fromString("boolean")}, VarType::fromString("void"), FilePermission::FilePermission_SetCanWrite);

}

bool FilePermission::isWriteMode(std::string mode)
{
    return mode == "w" || mode == "a";
}

bool FilePermission::isReadMode(std::string mode)
{
    return mode == "r";
}


void FilePermission::checkPermissionAllows(Interpreter* interpreter, Scope* caller_scope, std::string absolute_filename_path, std::string mode)
{
    // Let's ensure this scope has permission to open this file
    if (!interpreter->hasNoPermissionRestrictions())
    {
        // We don't have access yet let's see how this turns out
        bool has_access = false;

        std::vector<std::shared_ptr<PermissionObject>> permission_list = caller_scope->getPermissionList("FilePermission");
        // If the permission list is empty then we don't have permission to open this file
        if (permission_list.empty())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You don't have permission to open the file: " + absolute_filename_path, interpreter->getStackTraceLog());
        }
        for (std::shared_ptr<PermissionObject> perm : permission_list)
        {
            std::shared_ptr<FilePermission> permission = std::dynamic_pointer_cast<FilePermission>(perm);
            // Do we have access to the directory we are trying to access?
            if (startsWith(absolute_filename_path, permission->location->value.svalue))
            {
                if (FilePermission::isReadMode(mode))
                {
                    // If we can't read then just continue and hopefully we will find a permission that allows this
                    if (!permission->can_read->value.dvalue)
                    {
                        continue;
                    }
                }
                else if (FilePermission::isWriteMode(mode))
                {
                    // If we can't write then just continue and hopefully we will find a permission that allows this
                    if (!permission->can_write->value.dvalue)
                    {
                        continue;
                    }
                }
                has_access = true;
                break;
            }
        }

        if (!has_access)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You don't have the permission to access the path: " + absolute_filename_path + " for mode: " + mode, interpreter->getStackTraceLog());
        }
    }
}





void FilePermission::FilePermission_SetLocation(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FilePermission> file_perm_obj = std::dynamic_pointer_cast<FilePermission>(object);
    if (file_perm_obj->location->is_locked)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))), "", interpreter->getStackTraceLog());
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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))), "", interpreter->getStackTraceLog());
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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("VariableLockedException"))), "", interpreter->getStackTraceLog());
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
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "", interpreter->getStackTraceLog());
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
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions", interpreter->getStackTraceLog());
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
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You cannot escalate the permissions past your own permissions", interpreter->getStackTraceLog());
            }
        }
    }

}


void FilePermission::FilePermission_PriorAdd(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    
}


