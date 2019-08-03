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

#include "../include/tcpsocketobject.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "class.h"
#include "object.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "tcpsocketserverobject.h"
#include "interpreter.h"

TcpSocketObject::TcpSocketObject(Class *c) : SocketObject(c)
{
    this->socket_server = nullptr;
}
TcpSocketObject::~TcpSocketObject()
{
}

void TcpSocketObject::registerClass(ModuleSystem *moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("TcpSocket", moduleSystem->getClassSystem()->getClassByName("Socket"));
    c->setDescriptorObject(std::make_shared<TcpSocketObject>(c));

    c->registerFunction("__construct", {VarType::fromString("TcpSocketServer")}, VarType::fromString("void"), TcpSocketObject::TcpSocket__construct);

    /**
     * 
     * Sends the given data to the client
     * function send(string data) : void
     */
    c->registerFunction("send", {VarType::fromString("string")}, VarType::fromString("void"), TcpSocketObject::TcpSocket_send);
    /**
     * Reads the amount of bytes specified from the socket stream as a string
     * function recv(number amount) : string
     */
    c->registerFunction("recv", {VarType::fromString("number")}, VarType::fromString("string"), TcpSocketObject::TcpSocket_recv);
}

void TcpSocketObject::newInterpreter(Interpreter *interpreter)
{
}

std::shared_ptr<Object> TcpSocketObject::newInstance(Class *c)
{
    return std::make_shared<TcpSocketObject>(c);
}

void TcpSocketObject::TcpSocket__construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<TcpSocketObject> tcp_object = std::dynamic_pointer_cast<TcpSocketObject>(object);
    tcp_object->socket_server = std::dynamic_pointer_cast<TcpSocketServerObject>(values[0].ovalue);
}

void TcpSocketObject::TcpSocket_recv(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
}

void TcpSocketObject::TcpSocket_send(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{

    
}
