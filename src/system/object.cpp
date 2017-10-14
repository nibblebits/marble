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

