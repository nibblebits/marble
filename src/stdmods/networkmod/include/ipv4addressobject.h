#ifndef IPV4ADDRESSOBJECT_H
#define IPV4ADDRESSOBJECT_H

#include "object.h"
class ModuleSystem;
class Ipv4AddressObject : public Object
{
public:
    Ipv4AddressObject(Class* c);
    virtual ~Ipv4AddressObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);

    // Raw IP string that represents this Ipv4 address
    std::string ip_address;

    // Native Ipv4Address methods
    static void Ipv4Address_toString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};
#endif