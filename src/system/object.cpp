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
#include "object.h"
#include "variable.h"
#include "function.h"
#include "systemhandler.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptions/systemexception.h"
#include <iostream>
#include <memory>

Object::Object(Class *c) : Object(c, NULL)
{
    Variable *this_var = createVariable();
    this_var->type = VARIABLE_TYPE_OBJECT;
    this_var->type_name = getClass()->name;
    this_var->name = "this";
    this_var->value.holder = this_var;
    Variable *super_var = cloneCreate(this_var);
    super_var->name = "super";
}

Object::Object(Class *c, std::shared_ptr<PermissionsObject> permissions) : Scope(permissions)
{
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->sys_handler = c->getSystemHandler();

    if (this->sys_handler == NULL)
        throw std::logic_error("Attempting to create an object for the class " + c->name + " but this class does not have a SystemHandler");

    this->c = c;
    // Let's create variables for the object based by the class variables
    Class *current = c;
    while (current != NULL)
    {
        for (Variable v : current->getVariables())
        {
            cloneCreate(&v);
        }
        current = current->parent;
    }

    this->prev = NULL;

    this->is_running = false;
}

Object::~Object()
{
}

std::shared_ptr<Object> Object::create(Class *object_class)
{
    if (object_class == NULL)
    {
        throw std::logic_error("The class provided is NULL");
    }
    return object_class->getDescriptorObject()->newInstance();
}

std::shared_ptr<Object> Object::create(Interpreter *interpreter, Class *object_class, std::vector<Value> constructor_values)
{
    if (object_class == NULL)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("EntityNotRegisteredException"), {})), "The class provided has not been registered", interpreter->getStackTraceLog());
    }

    std::shared_ptr<Object> object = object_class->getDescriptorObject()->newInstance();

    // The constructor must now be called
    Function *constructor = object_class->getFunctionByName("__construct");

    // The caller scope will be the scope before invoking the constructor
    Scope *caller_scope = interpreter->getCurrentScope();

    // Before calling the function we must also setup the permissions for this object. The permissions will be inherited from the scope thats creating this object
    object->permissions = caller_scope->permissions;
    if (constructor != NULL)
    {
        object->runThis([&] {
            // Invoke that constructor!
            constructor->invoke(interpreter, constructor_values, NULL, object, caller_scope);
        },
                        interpreter, constructor->cls);
    }

    return object;
}

std::shared_ptr<Object> Object::create(Interpreter *interpreter, std::string object_class, std::vector<Value> constructor_values)
{
    Class *cls = interpreter->getClassSystem()->getClassByName(object_class);
    if (cls == NULL)
        throw std::logic_error("Object::create the class name provided " + object_class + " no class has been registered with that name yet");

    return Object::create(interpreter, cls, constructor_values);
}

void Object::registerVariable(Variable *variable)
{
    if (variable == NULL)
        throw std::logic_error("The variable must not be NULL");

    variable->object = this;
    Scope::registerVariable(variable);
}

Class *Object::getClass()
{
    return this->c;
}

Class *Object::getClass(std::string name)
{
    Class *current = getClass();
    if (!current->instanceOf(name))
        throw std::logic_error("This object is not an instance of the class: " + name);

    while (current != NULL)
    {
        if (current->name == name)
            return current;

        current = current->parent;
    }
}

std::shared_ptr<Object> Object::newInstance()
{
    return newInstance(getClass());
}

std::shared_ptr<Object> Object::newInstance(Class *c)
{
    return std::make_shared<Object>(c);
}

bool Object::isRunning()
{
    return this->is_running;
}

void Object::runThis(std::function<void()> function, SystemHandler *sys_handler, Class *c, OBJECT_ACCESS_TYPE access_type, Scope *accessors_scope)
{
    newRun();

    FunctionSystem *old_fc_system;
    Scope *old_scope;
    if (c == NULL)
    {
        c = this->getClass();
    }
    else
    {
        // Let's ensure that this class is based on this object.
        if (!getClass()->instanceOf(c))
            throw std::logic_error("The class provided is not related to the class \"" + this->getClass()->name + "\". The class provided is: " + c->name);
    }

    if (accessors_scope == NULL)
    {
        // No accessors scope is provided so we will default to the current scope
        accessors_scope = sys_handler->getCurrentScope();
    }

    old_fc_system = sys_handler->getFunctionSystem();
    old_scope = sys_handler->getCurrentScope();
    if (access_type == OBJECT_ACCESS_TYPE_OBJECT_ACCESS)
        sys_handler->setCurrentObject(shared_from_this(), c);
    Scope *old_prev_scope = this->prev;

    // We must set our objects previous scope to the system handlers global scope so global variables are recognized
    this->prev = sys_handler->getGlobalScope();
    
    sys_handler->setCurrentScope(this);
    sys_handler->setFunctionSystem(c);

    try
    {
        // Now the scopes have been adjusted to run on this object we should invoke the function provided
        function();
    }
    catch (...)
    {
        // and now restore
        if (access_type == OBJECT_ACCESS_TYPE_OBJECT_ACCESS)
            sys_handler->finishCurrentObject();
        sys_handler->setCurrentScope(old_scope);
        sys_handler->setFunctionSystem(old_fc_system);
        if (old_scope != this)
        {
            this->prev = old_prev_scope;
        }
        finishRun();
        throw;
    }

    // and now restore
    if (access_type == OBJECT_ACCESS_TYPE_OBJECT_ACCESS)
        sys_handler->finishCurrentObject();
    sys_handler->setCurrentScope(old_scope);
    sys_handler->setFunctionSystem(old_fc_system);

    // Let's restore our previous scope
    this->prev = old_prev_scope;
    
    finishRun();
}

void Object::onEnterScope(bool tell_parents)
{
    Scope::onEnterScope(tell_parents);
    // Let's create the "this" and "super" variables when entering scope
    Variable *this_var = getVariable("this");
    Variable *super_var = getVariable("super");
    this_var->value.ovalue = shared_from_this();
    super_var->value.ovalue = shared_from_this();
}

void Object::onLeaveScope(bool tell_parents)
{
    Scope::onLeaveScope(tell_parents);

    /* To avoid memory leaks we need to remove the object references for this and super.
    */
    Variable *this_var = getVariable("this");
    Variable *super_var = getVariable("super");
    this_var->value.ovalue = NULL;
    super_var->value.ovalue = NULL;
}

void Object::newRun()
{
    this->run_stack.push_back(this->is_running);
    this->is_running = true;
}

void Object::finishRun()
{
    this->is_running = this->run_stack.back();
    this->run_stack.pop_back();
}