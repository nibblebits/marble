#include "../include/socketobject.h"

SocketObject::SocketObject(Class* c) : Object(c)
{

}

SocketObject::~SocketObject()
{

}

std::shared_ptr<Object> SocketObject::newInstance(Class* c)
{
    return std::make_shared<SocketObject>(c);
}