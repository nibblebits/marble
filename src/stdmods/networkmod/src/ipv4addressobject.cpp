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

#include "../include/ipv4addressobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"

Ipv4AddressObject::Ipv4AddressObject(Class *c) : Object(c)
{
}

Ipv4AddressObject::~Ipv4AddressObject()
{
}

std::shared_ptr<Object> Ipv4AddressObject::newInstance(Class *c)
{
    return std::make_shared<Ipv4AddressObject>(c);
}

void Ipv4AddressObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Ipv4Address
     * 
     * The Ipv4Address holds an Ipv4 IP address that you can access
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Ipv4Address");
    c->setDescriptorObject(std::make_shared<Ipv4AddressObject>(c));

    /**
     * @class Ipv4Address
     * 
     * Constructs this Ipv4Address object. The string provided must be the Ipv4 address in a string form 255.255.255.255
     */
    c->registerFunction("__construct", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        std::shared_ptr<Ipv4AddressObject> ipv4_address = std::dynamic_pointer_cast<Ipv4AddressObject>(object);
        ipv4_address->ip_address = values[0].svalue;
    });

    /**
     * @class Ipv4Address
     * 
     * Returns the IP address as a string
     * function toString() : string
     */
    c->registerFunction("toString", {}, {VarType::fromString("string")}, Ipv4AddressObject::Ipv4Address_toString);
}

void Ipv4AddressObject::Ipv4Address_toString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<Ipv4AddressObject> ipv4_address = std::dynamic_pointer_cast<Ipv4AddressObject>(object);
    return_value->set(ipv4_address->ip_address);
}