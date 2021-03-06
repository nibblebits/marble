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

#include "basesystemhandler.h"
#include "exceptionobject.h"
#include "permissionobject.h"
#include "permissionsobject.h"
#include "permissionpropertyobject.h"
#include "modulehandlingpermissionobject.h"
#include "filepermission.h"
#include "array.h"
#include "function.h"
BaseSystemHandler::BaseSystemHandler() : SystemHandler(SYSTEM_HANDLER_BASE_SYSTEM_HANDLER, NULL, NULL)
{

    // Register the Object class
    /**
     * class Object
     * 
     * This highest possible base class. All objects are an instance of this object.
     */
    Class* c = getClassSystem()->registerClass("Object", NULL, CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER);
    /**
     * @class Object
     * 
     * Constructs this Object
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);
    /**
     * @class Object
     * 
     * The toString method is to be overrided by any class wishing to handle toString functionality.
     * This method should be called when you want a string representation of the Object you called it on.
     * The class of the object instance you called it on is required to implement toString() and provide a string
     * representation of their object
     *
     * function toString() : void
     */
    c->registerFunction("toString", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<Array> array = std::dynamic_pointer_cast<Array>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = "toString() for object not implemented";
    });
    getClassSystem()->setDefaultBaseClass(c);

     /* Let's register an Exception class that is to be inherited by all classes that can be thrown*/
     /**
        * class Exception
        * The Exception class is the highest possible class for exceptions. All exceptions are an instance of this Exception class.
        * Extend this class to create an exception of your own
        */
    Class* exception_class = getClassSystem()->registerClass("Exception");
    /**
     * @class Exception
     * 
     * Constructs this Exception class
     * function __construct() : void
     */
    exception_class->registerFunction("__construct", { }, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        // Blank constructor so just call the parent
        object->getClass()->invokeObjectParentConstructor({}, object, interpreter);
    });
    /**
     * @class Exception
     * 
     * Constructs this Exception class and sets the exception message to the message provided
     * function __construct(string message) : void
     */
    exception_class->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> except_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        except_obj->setMessage(arguments[0].svalue);
    });

    /**
     * @class Exception
     * 
     * Returns the message associated with this Exception if no message was set it just returns a blank string ""
     * function getMessage() : string
     */
    exception_class->registerFunction("getMessage", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> except_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        return_value->set(except_obj->getMessage());
    });
    
    /**
     * @class Exception
     * Returns the stack trace of this Exception as a string. The stack trace allows you to trace the origin of this Exception
     * function getStackTrace() : string
     */
    exception_class->registerFunction("getStackTrace", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<ExceptionObject> exception_obj = std::dynamic_pointer_cast<ExceptionObject>(object);
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue = exception_obj->getStackTrace();
    });
    // The Exception class has the Exception object instance.
    exception_class->setDescriptorObject(std::make_shared<ExceptionObject>(exception_class));

    // Create a permissions exception
    /**
     * class PermissionException
     * The PermissionException class is thrown when ever a permission violation has occured. When implementing your own permissions 
     * you throw this exception if the user is trying to do something they are not authorized to do
     */
    c = getClassSystem()->registerClass("PermissionException", exception_class);
    /**
     * @class PermissionException
     * 
     * Constructs this PermissionException with the given error message
     * function __construct(string message) : void
     */   
    c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
         object->getClass()->invokeObjectParentConstructor(arguments, object, interpreter);
    });



    // We need a permission class to help manage permissions
    PermissionObject::registerClass(this);

    // We also need a permissions class that will hold the permissions for a current scope
    PermissionsObject::registerClass(this);

    // We need to register the PermissionProperty class which will hold a variable name and value for a permission variable.
    PermissionPropertyObject::registerClass(this);


    // Register the FilePermission class
    FilePermission::registerClass(this);
    
    // We need a module handling permissions object that will allow a marble programmer to load modules
    ModuleHandlingPermissionObject::registerClass(this);
}

BaseSystemHandler::~BaseSystemHandler()
{

}
