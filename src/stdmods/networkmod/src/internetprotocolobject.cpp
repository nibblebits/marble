#include "../include/internetprotocolobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "ipv4addressobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "object.h"
#include <sys/socket.h>
#include <errno.h> //For errno - the error number
#include <netdb.h> //hostent
#include <arpa/inet.h>

InternetProtocolObject::InternetProtocolObject(Class *c) : Object(c)
{
}

InternetProtocolObject::~InternetProtocolObject()
{
}

std::shared_ptr<Object> InternetProtocolObject::newInstance(Class *c)
{
    return std::make_shared<InternetProtocolObject>(c);
}

void InternetProtocolObject::registerClass(ModuleSystem *moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("InternetProtocol");
    c->setDescriptorObject(std::make_shared<InternetProtocolObject>(c));

    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class InternetProtocol
     * 
     * Resolves the IP address for the given hostname and returns an Ipv4Address representing the IP address of the hostname
     * function getAddressForHostname(string hostname) : Ipv4Address
     */
    c->registerFunction("getAddressForHostname", {VarType::fromString("string")}, VarType::fromString("Ipv4Address"), InternetProtocolObject::InternetProtocol_getAddressForHostname);
}

void InternetProtocolObject::InternetProtocol_getAddressForHostname(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    struct hostent *he;
    struct in_addr **addr_list;
    std::string hostname = values[0].svalue;

    if ((he = gethostbyname(hostname.c_str())) == NULL)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Issue resolving hostname");
    }

    std::string resulting_ip = "";
    addr_list = (struct in_addr **)he->h_addr_list;
    if (addr_list[0] != NULL)
    {
        std::string resulting_ip = inet_ntoa(*addr_list[0]);
        if (resulting_ip == "")
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Domain name could not be resolved");
        }
        
        // Let's now create an Ipv4Address object to store this IP address in
        std::shared_ptr<Ipv4AddressObject> ipv4_address = std::dynamic_pointer_cast<Ipv4AddressObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Ipv4Address"), {resulting_ip}));
        return_value->set(ipv4_address);
    }

}