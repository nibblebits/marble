#include "object.h"
#include <iostream>
Object::Object(Class* c)
{
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->c = c;
}

Object::~Object()
{
}

Class* Object::getClass()
{
    return this->c;
}

