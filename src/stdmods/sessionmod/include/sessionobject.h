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

    // Native Session functions/methods
    static void Session_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif