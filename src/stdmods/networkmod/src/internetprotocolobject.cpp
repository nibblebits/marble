/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "../include/internetprotocolobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "ipv4addressobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "networkpermission.h"
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
    /**
     * class InternetProtocol
     * 
     * The InternetProtocol class is responsible for resolving hostnames to ip addresses
     */
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
    // Ensure that we have network permissions
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_LOOKUP_PERMISSION_REQUIRED);
    
    struct hostent *he;
    struct in_addr **addr_list;
    std::string hostname = values[0].svalue;

    if ((he = gethostbyname(hostname.c_str())) == NULL)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Issue resolving hostname", interpreter->getStackTraceLog());
    }

    std::string resulting_ip = "";
    addr_list = (struct in_addr **)he->h_addr_list;
    if (addr_list[0] != NULL)
    {
        std::string resulting_ip = inet_ntoa(*addr_list[0]);
        if (resulting_ip == "")
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Domain name could not be resolved", interpreter->getStackTraceLog());
        }
        
        // Let's now create an Ipv4Address object to store this IP address in
        std::shared_ptr<Ipv4AddressObject> ipv4_address = std::dynamic_pointer_cast<Ipv4AddressObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Ipv4Address"), {resulting_ip}));
        return_value->set(ipv4_address);
    }

}