#include "object.h"
#include "variable.h"
#include "function.h"
#include "systemhandler.h"
#include <iostream>
#include <memory>
Object::Object(Class* c)
{
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->sys_handler = c->getSystemHandler();
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
    prev = sys_handler->getRootScope();
    
}

Object::~Object()
{
}


std::shared_ptr<Object> Object::create(Class* object_class, std::vector<Value> constructor_values)
{
    std::shared_ptr<Object> object = std::make_shared<Object>(object_class);
    
    // The constructor must now be called
    Function* constructor = object_class->getFunctionByName("__construct");
    if (constructor != NULL)
    {
        object->runThis([&]
        {
            // Invoke that constructor!
            constructor->invoke(constructor_values, NULL, object);
        }, constructor->cls);
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


void Object::runThis(std::function<void()> function, Class* c)
{
    FunctionSystem* old_fc_system;
    Scope* old_scope;
    std::shared_ptr<Object> old_obj;
    if (c == NULL)
    {
        c = this->getClass();
    }
    else
    {
        // Let's ensure that this class is based on this object.
        if (!this->getClass()->instanceOf(c))
            throw std::logic_error("The class provided is not related to the class \"" + this->getClass()->name + "\"");
    }  
    old_fc_system = sys_handler->getFunctionSystem();
    old_scope = sys_handler->getCurrentScope();
    old_obj = sys_handler->getCurrentObject();
    sys_handler->setCurrentObject(shared_from_this());
    sys_handler->setCurrentScope(this);
    sys_handler->setFunctionSystem(c);
    
    // Now the scopes have been adjusted to run on this object we should invoke the function provided
    function();
    
    // and now restore
    sys_handler->setCurrentObject(old_obj);
    sys_handler->setCurrentScope(old_scope);
    sys_handler->setFunctionSystem(old_fc_system);
    
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

