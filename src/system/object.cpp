#include "object.h"
#include "variable.h"
#include "systemhandler.h"
#include <iostream>
#include <memory>
Object::Object(SystemHandler* sys_handler, Class* c)
{
    this->sys_handler = sys_handler;
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
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

