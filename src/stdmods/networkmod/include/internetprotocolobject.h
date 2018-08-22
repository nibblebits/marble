#ifndef INTERNETPROTOCOL_H
#define INTERNETPROTOCOL_H

#include "object.h"
class ModuleSystem;
class InternetProtocolObject : public Object
{
public:
    InternetProtocolObject(Class* c);
    virtual ~InternetProtocolObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);

    // Native Internet Protocol methods
    static void InternetProtocol_getAddressForHostname(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif