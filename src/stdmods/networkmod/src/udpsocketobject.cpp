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

#include "../include/udpsocketobject.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "class.h"
#include "object.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "interpreter.h"
UdpSocketObject::UdpSocketObject(Class *c) : SocketObject(c)
{

}

UdpSocketObject::~UdpSocketObject()
{
}


void UdpSocketObject::newInterpreter(Interpreter *interpreter)
{

}


void UdpSocketObject::registerClass(ModuleSystem *moduleSystem)
{

    Class *c = moduleSystem->getClassSystem()->registerClass("UdpSocket", moduleSystem->getClassSystem()->getClassByName("Socket"));
    c->setDescriptorObject(std::make_shared<UdpSocketObject>(c));

    c->registerFunction("__construct", {}, VarType::fromString("void"), UdpSocketObject::UdpSocket__construct);

    /**
     * @class Socket
     * 
     * Binds the socket to the given port
     * function bind(number port) : void
     */
    c->registerFunction("bind", {VarType::fromString("number")}, VarType::fromString("void"), UdpSocketObject::UdpSocket_bind);
    c->registerFunction("recv", {VarType::fromString("number")}, VarType::fromString("string"), UdpSocketObject::UdpSocket_recv);
    c->registerFunction("send", {VarType::fromString("Ipv4Address"), VarType::fromString("number"), VarType::fromString("string")}, VarType::fromString("void"), UdpSocketObject::UdpSocket_send);
}

void UdpSocketObject::UdpSocket__construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<UdpSocketObject> s_object = std::dynamic_pointer_cast<UdpSocketObject>(object);
    s_object->family = AF_INET;
    s_object->type = SOCK_DGRAM;
    s_object->sockfd = socket(s_object->family, s_object->type, 0);
}

std::shared_ptr<Object> UdpSocketObject::newInstance(Class *c)
{
    return std::make_shared<UdpSocketObject>(c);
}

void UdpSocketObject::UdpSocket_bind(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<UdpSocketObject> s_object = std::dynamic_pointer_cast<UdpSocketObject>(object);
    struct sockaddr_in servaddr;
    servaddr.sin_family = SOCK_DGRAM; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(values[0].dvalue);
    if (bind(s_object->sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "binding failed", interpreter->getStackTraceLog());
    }
}

void UdpSocketObject::UdpSocket_send(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<UdpSocketObject> s_object = std::dynamic_pointer_cast<UdpSocketObject>(object);
    std::shared_ptr<Object> ipv4_obj = values[0].ovalue;
    Value tmp_val;
    Function *ipv4_obj_tostring = ipv4_obj->getClass()->getFunctionByName("toString");
    ipv4_obj_tostring->invoke(interpreter, {}, &tmp_val, ipv4_obj, interpreter->getCurrentScope());
    std::string ipv4_address = tmp_val.svalue;
    int port = values[1].dvalue;
    std::string data = values[2].svalue;

    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr(ipv4_address.c_str());
    cliaddr.sin_port = htons(port);

    if (sendto(s_object->sockfd, data.c_str(), data.size(), 0, (const struct sockaddr *)&cliaddr, sizeof(cliaddr)) < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Problem sending UDP packet", interpreter->getStackTraceLog());
    }
}

void UdpSocketObject::UdpSocket_recv(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<UdpSocketObject> s_object = std::dynamic_pointer_cast<UdpSocketObject>(object);
    socklen_t total_to_read = values[0].dvalue;
    socklen_t sockaddr_len = sizeof(struct sockaddr_in);

    char buf[total_to_read];

    struct sockaddr cliaddr;
    int number_read = recvfrom(s_object->sockfd, (char *)buf, total_to_read,
                               MSG_WAITALL, (struct sockaddr *)&cliaddr,
                               &sockaddr_len);

    std::string return_val = "";
    for (int i = 0; i < total_to_read; i++)
    {
        return_val += buf[i];
    }

    return_value->set(return_val);
}