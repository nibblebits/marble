#include "socketaddressobject.h"  
  
SocketAddressObject::SocketAddressObject(Class* c) : Object(c)
{
    this->cli_len = -1;
}

SocketAddressObject::~SocketAddressObject()
{

}

std::shared_ptr<Object> SocketAddressObject::newInstance(Class* c)
{
    return std::make_shared<SocketAddressObject>(c);
}


void SocketAddressObject::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class SocketAddress
     * 
     * Represents a socket address, does not allow interaction currently and is used for internal use.
     * This will be changed soon and a more extensive class will be provided
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("SocketAddress", moduleSystem->getClassSystem()->getClassByName("Object"));
    c->setDescriptorObject(std::make_shared<SocketAddressObject>(c));

    /**
     * @class SocketAddress
     * 
     * Constructs this SocketAddress object
     */
    c->registerFunction("__construct", {}, {VarType::fromString("void")}, SocketAddressObject::SocketAddress_Construct);

}

void SocketAddressObject::SocketAddress_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{

}



