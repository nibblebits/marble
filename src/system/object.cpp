#include "object.h"
#include <iostream>
Object::Object(Class* c)
{
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->c = c;
    this->obj_scope = std::make_unique<Scope>();
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

