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