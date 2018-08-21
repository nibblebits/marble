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

void SocketObject::Socket_connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    
}