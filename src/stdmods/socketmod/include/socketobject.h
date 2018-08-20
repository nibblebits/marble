#ifndef SOCKETOBJECT_H
#define SOCKETOBJECT_H

#include "object.h"
class SocketObject : public Object
{
public:
    SocketObject(Class* c);
    virtual ~SocketObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);


};
#endif