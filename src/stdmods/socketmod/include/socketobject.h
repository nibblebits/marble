#ifndef SOCKETOBJECT_H
#define SOCKETOBJECT_H

#include "object.h"
class SocketObject : public Object
{
public:
    SocketObject(Class* c);
    virtual ~SocketObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    // Native Socket methods
    static void Socket_connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif