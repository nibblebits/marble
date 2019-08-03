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

#include "../include/tcpsocketserverobject.h"
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
#include "network_config.h"
TcpSocketServerObject::TcpSocketServerObject(Class *c) : SocketObject(c)
{
    this->sockfd = 0;
}

TcpSocketServerObject::~TcpSocketServerObject()
{
}

void TcpSocketServerObject::newInterpreter(Interpreter *interpreter)
{
}

void TcpSocketServerObject::registerClass(ModuleSystem *moduleSystem)
{

    Class *c = moduleSystem->getClassSystem()->registerClass("TcpSocketServer", moduleSystem->getClassSystem()->getClassByName("Socket"));
    c->setDescriptorObject(std::make_shared<TcpSocketServerObject>(c));
    
    c->registerFunction("__construct", {}, VarType::fromString("void"), TcpSocketServerObject::TcpSocketServerObject__construct);
    c->registerFunction("listen", {VarType::fromString("number")}, VarType::fromString("void"), TcpSocketServerObject::TcpSocketServerObject_listen);
    c->registerFunction("accept", {}, VarType::fromString("void"), TcpSocketServerObject::TcpSocketServerObject_accept);
}

void TcpSocketServerObject::TcpSocketServerObject__construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<TcpSocketServerObject> s_object = std::dynamic_pointer_cast<TcpSocketServerObject>(object);
    s_object->family = AF_INET;
    s_object->type = SOCK_STREAM;
    s_object->sockfd = socket(s_object->family, s_object->type, 0);

    if (s_object->sockfd < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Failed to create socket", interpreter->getStackTraceLog());
    }
}

std::shared_ptr<Object> TcpSocketServerObject::newInstance(Class *c)
{
    return std::make_shared<TcpSocketServerObject>(c);
}

void TcpSocketServerObject::TcpSocketServerObject_listen(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<TcpSocketServerObject> s_object = std::dynamic_pointer_cast<TcpSocketServerObject>(object);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(values[0].dvalue);
    s_object->addr = servaddr;

    if (bind(s_object->sockfd, (const sockaddr *)&s_object->addr, sizeof(s_object->addr)) < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Listening failed due to bind failure", interpreter->getStackTraceLog());
    }

    if (listen(s_object->sockfd, MAX_LISTEN_CONNECTIONS) < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Listening failed maybe we are waiting on too many connections", interpreter->getStackTraceLog());
    }
}

void TcpSocketServerObject::TcpSocketServerObject_accept(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<TcpSocketServerObject> s_object = std::dynamic_pointer_cast<TcpSocketServerObject>(object);
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    memset(&cli_addr, 0, sizeof(cli_addr));
    if (accept(s_object->sockfd, (struct sockaddr *)&cli_addr, &cli_len) < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Accepting of the connection has failed", interpreter->getStackTraceLog());
    }
}