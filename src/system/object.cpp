#include "object.h"
#include <iostream>
Object::Object(Class* c)
{
    this->c = c;
}

Object::~Object()
{
}

Class* Object::getClass()
{
    return this->c;
}

