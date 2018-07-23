#ifndef SESSIONOBJECT_H
#define SESSIONOBJECT_H

#include "sessionvaluesobject.h"
class ModuleSystem;
class SessionObject : public SessionValuesObject
{
public:
    SessionObject(Class* c);
    virtual ~SessionObject();

    static void registerClass(ModuleSystem* moduleSystem);
    virtual std::shared_ptr<Object> newInstance(Class* c);

};

#endif