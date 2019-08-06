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
#include <unistd.h>
#include "class.h"
#include "object.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "tcpsocketserverobject.h"
#include "networkpermission.h"
#include "interpreter.h"

TcpSocketObject::TcpSocketObject(Class *c) : SocketObject(c)
{
    this->socket_server = nullptr;
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0); 
}
TcpSocketObject::~TcpSocketObject()
{
}

void TcpSocketObject::registerClass(ModuleSystem *moduleSystem)
{

    /**
     * class TcpSocket extends Socket
     * 
     * Represents a TCP Socket connection, this connection can be unconnected in which case call connect.
     * If you wish to spawn a raw TCP connection create a new instance of this class and then call the connect method
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("TcpSocket", moduleSystem->getClassSystem()->getClassByName("Socket"));
    c->setDescriptorObject(std::make_shared<TcpSocketObject>(c));

    /*
    * @class TcpSocket
    * Constructs this TcpSocket with a socket server and socket address
    * function __construct(TcpSocketServer socket_server, SocketAddress socket_address) : void
    */
    c->registerFunction("__construct", {VarType::fromString("TcpSocketServer"), VarType::fromString("SocketAddress")}, VarType::fromString("void"), TcpSocketObject::TcpSocket__constructForServer);
    /**
     * @class TcpSocket
     * Constructs this TcpSocket with no parameters
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), TcpSocketObject::TcpSocket__construct);

    /**
     * @class TcpSocket
     * Connects to the provided Ipv4 address and port
     * function connect(Ipv4Address address, number port) : void
     */
    c->registerFunction("connect", {VarType::fromString("Ipv4Address"), VarType::fromString("number")}, VarType::fromString("void"), TcpSocketObject::TcpSocket_connect);

    /**
     * @class TcpSocket
     * Sends the given data string amount to the client
     * data string variable can contains binary data this is fine, null terminator is not monitored, pass correct amount of data to send
     * if this is a text string pass the size of the string in the amount
     * function send(string data, number amount) : number
     */
    c->registerFunction("send", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("number"), TcpSocketObject::TcpSocket_send);
    /**
     * @class TcpSocket
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
    // An empty space with nothing inside
}

void TcpSocketObject::TcpSocket_connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{

    // Ensure we have permission
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_SOCKET_PERMISSION_REQUIRED);
    
    std::shared_ptr<TcpSocketObject> tcp_object = std::dynamic_pointer_cast<TcpSocketObject>(object);
    std::shared_ptr<Object> ipv4_obj = values[0].ovalue;
    Value tmp_val;
    Function *ipv4_obj_tostring = ipv4_obj->getClass()->getFunctionByName("toString");
    ipv4_obj_tostring->invoke(interpreter, {}, &tmp_val, ipv4_obj, interpreter->getCurrentScope());
    std::string ipv4_address = tmp_val.svalue;
    int port = values[1].dvalue;

    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_addr.s_addr = inet_addr(ipv4_address.c_str());
    cliaddr.sin_port = htons(port);
    socklen_t cli_len = sizeof(cliaddr);
    if (connect(tcp_object->sockfd, (const struct sockaddr*) &cliaddr, cli_len) < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Failed to connect to server", interpreter->getStackTraceLog());
    }



}

void TcpSocketObject::TcpSocket__constructForServer(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<TcpSocketObject> tcp_object = std::dynamic_pointer_cast<TcpSocketObject>(object);
    tcp_object->socket_server = std::dynamic_pointer_cast<TcpSocketServerObject>(values[0].ovalue);
    tcp_object->socket_address = std::dynamic_pointer_cast<SocketAddressObject>(values[1].ovalue);
}

void TcpSocketObject::TcpSocket_recv(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Ensure we have permission
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_SOCKET_PERMISSION_REQUIRED);

    std::shared_ptr<TcpSocketObject> s_object = std::dynamic_pointer_cast<TcpSocketObject>(object);
    socklen_t total_to_read = values[0].dvalue;
    char buf[total_to_read];
    memset(buf, 0, total_to_read);

    int number_read = read(s_object->sockfd, (char *)buf, total_to_read);
    if (number_read < 0)
    {
         throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Tcp Read failed", interpreter->getStackTraceLog());       
    }
    std::string return_val = "";
    for (int i = 0; i < total_to_read; i++)
    {
        return_val += buf[i];
    }

    return_value->set(return_val);
}

void TcpSocketObject::TcpSocket_send(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Ensure we have permission
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_SOCKET_PERMISSION_REQUIRED);

    std::shared_ptr<TcpSocketObject> s_object = std::dynamic_pointer_cast<TcpSocketObject>(object);
    socklen_t total_to_write = values[1].dvalue;
    if (total_to_write >= values[0].svalue.size() || total_to_write < 0)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SocketException"))), "Provided data size is out of bounds for the data string provided", interpreter->getStackTraceLog());
    }

    int number_read = send(s_object->sockfd, values[0].svalue.c_str(), total_to_write, 0);

    return_value->set(number_read);
}
