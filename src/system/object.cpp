#include "object.h"
#include "variable.h"
#include "function.h"
#include "systemhandler.h"
#include "interpreter.h"
#include <iostream>
#include <memory>
Object::Object(Class* c)
{
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->sys_handler = c->getSystemHandler();

    if (this->sys_handler == NULL)
        throw std::logic_error("Attempting to create an object for the class " + c->name + " but this class does not have a SystemHandler");

    this->c = c;
    // Let's create variables for the object based by the class variables
    Class* current = c;
    while(current != NULL)
    {
        for (Variable v : current->getVariables())
        {
            cloneCreate(&v);
        }
        current = current->parent;
    }

    if (this->sys_handler != NULL)
        prev = sys_handler->getRootScope();

    this->is_running = false;
    
}

Object::~Object()
{
}

std::shared_ptr<Object> Object::create(Class* object_class)
{
    return object_class->getDescriptorObject()->newInstance();
}

std::shared_ptr<Object> Object::create(Interpreter* interpreter, Class* object_class, std::vector<Value> constructor_values)
{
    std::shared_ptr<Object> object = object_class->getDescriptorObject()->newInstance();
    // The constructor must now be called
    Function* constructor = object_class->getFunctionByName("__construct");
    if (constructor != NULL)
    {
        object->runThis([&]
        {
            // Invoke that constructor!
            constructor->invoke(interpreter, constructor_values, NULL, object);
        }, interpreter, constructor->cls);
    }
    
    return object;
}

void Object::registerVariable(Variable* variable)
{
    if (variable == NULL)
        throw std::logic_error("The variable must not be NULL");
    
    variable->object = this;
    Scope::registerVariable(variable);
}

Class* Object::getClass()
{
    return this->c;
}


std::shared_ptr<Object> Object::newInstance()
{
    return newInstance(getClass());
}

std::shared_ptr<Object> Object::newInstance(Class* c)
{
    return std::make_shared<Object>(c);
}

bool Object::isRunning()
{
    return this->is_running;
}


void Object::invokeParentConstructor(Interpreter* interpreter, std::vector<Value> values)
{
    // Ignored return value as its a constructor
    Value return_value;
    Function* parent_constructor = this->getClass()->parent->getFunctionByName("__construct");
    if (parent_constructor != NULL)
    {
        parent_constructor->invoke(interpreter, values, &return_value, shared_from_this());
    }
}

void Object::runThis(std::function<void()> function, SystemHandler* sys_handler, Class* c, OBJECT_ACCESS_TYPE access_type, Scope* accessors_scope)
{
    newRun();

    FunctionSystem* old_fc_system;
    Scope* old_scope;
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
    sys_handler->setCurrentScope(this);
    sys_handler->setFunctionSystem(c);
    
    try
    {
        // Now the scopes have been adjusted to run on this object we should invoke the function provided
        function();
    }
    catch(...)
    {
        // and now restore
        if (access_type == OBJECT_ACCESS_TYPE_OBJECT_ACCESS)
            sys_handler->finishCurrentObject();
        sys_handler->setCurrentScope(old_scope);
        sys_handler->setFunctionSystem(old_fc_system);
        finishRun();
        throw;
    }
    
    // and now restore
    if (access_type == OBJECT_ACCESS_TYPE_OBJECT_ACCESS)
        sys_handler->finishCurrentObject();
    sys_handler->setCurrentScope(old_scope);
    sys_handler->setFunctionSystem(old_fc_system);

    finishRun();
    
}

void Object::onEnterScope()
{
    // Let's create the "this" and "super" variables when entering scope
    Variable* variable = createVariable();
    variable->type = VARIABLE_TYPE_OBJECT;
    variable->type_name = getClass()->name;
    variable->name = "this";
    variable->value.ovalue = shared_from_this();
    variable->value.holder = variable;

    variable = cloneCreate(variable);
    variable->name = "super";
}

void Object::onLeaveScope()
{
    /* To avoid memory leaks we need to remove the "this" and "super" variables upon leaving scope. This is because the shared_ptr will point to its self otherwise.
    * another reason for removing these variables is to prevent outside access to them.
    */
    removeVariable(getVariable("this"));
    removeVariable(getVariable("super"));
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