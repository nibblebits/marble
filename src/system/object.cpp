#include "object.h"
#include "variable.h"
#include <iostream>
Object::Object(Class* c)
{
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->c = c;
    this->obj_scope = std::make_unique<Scope>();
    // Let's create variables for the object based by the class variables
    for (Variable v : c->getVariables())
    {
        this->obj_scope->cloneCreate(&v);
    }
}

Object::~Object()
{
}


Scope* Object::getScope()
{
    return this->obj_scope.get();
}

Class* Object::getClass()
{
    return this->c;
}

