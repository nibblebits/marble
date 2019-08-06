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

#include "networkmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include "internetprotocolobject.h"
#include "ipv4addressobject.h"
#include "curlobject.h"
#include "mailobject.h"
#include "smtpmailobject.h"
#include "sendmailobject.h"
#include "socketobject.h"
#include "udpsocketobject.h"
#include "tcpsocketserverobject.h"
#include "tcpsocketobject.h"
#include "socketaddressobject.h"
#include "networkpermission.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <curl/curl.h>
#include <unistd.h>

NetworkModule::NetworkModule() : Module("networkmod", "Network Module", MODULE_TYPE_MARBLE_LIBRARY, "1.1.0", "https://marble.wiki/api/module/networkmod")
{
}

NetworkModule::~NetworkModule()
{
}

size_t NetworkModule::CurlReadCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    std::string *data_ptr = (std::string *)userp;
    if (data_ptr->empty())
        return 0;

    size_t amount_to_read = nmemb * size;
    if (amount_to_read > data_ptr->size())
        amount_to_read = data_ptr->size();

    memcpy(contents, data_ptr->c_str(), amount_to_read);
    // Let's erase the part of the string we have read
    data_ptr->erase(data_ptr->begin(), data_ptr->begin() + amount_to_read);

    return amount_to_read;
}

size_t NetworkModule::CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void NetworkModule::Init()
{

    // Let's setup CURL for use in other objects in the NetworkModule
    curl_global_init(CURL_GLOBAL_ALL);

    // Register the NetworkPermission class
    NetworkPermission::registerClass(this->getModuleSystem());

    // Register the Ipv4Address class
    Ipv4AddressObject::registerClass(this->getModuleSystem());

    // Register the InternetProtocol class
    InternetProtocolObject::registerClass(this->getModuleSystem());

    // Register the Curl class
    CurlObject::registerClass(this->getModuleSystem());

    // Register the Mail class
    MailObject::registerClass(this->getModuleSystem());

    // Regiser the SmtpMail class
    SmtpMailObject::registerClass(this->getModuleSystem());

    // Register the SendMail class
    SendMailObject::registerClass(this->getModuleSystem());

    // Register the SocketAddress class
    SocketAddressObject::registerClass(this->getModuleSystem());

    // Register the Socket class
    SocketObject::registerClass(this->getModuleSystem());

    // Register the UdpSocket class
    UdpSocketObject::registerClass(this->getModuleSystem());

    // Register the TcpSocketServer class
    TcpSocketServerObject::registerClass(this->getModuleSystem());

    // Register the TcpSocket class
    TcpSocketObject::registerClass(this->getModuleSystem());

}

void NetworkModule::newInterpreter(Interpreter *interpreter)
{
    Scope *root_scope = interpreter->getRootScope();
    root_scope->createVariable("InternetProtocol", "InternetProtocol", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("InternetProtocol"), {}));

    // Tell the CurlObject about this new interpreter
    CurlObject::newInterpreter(interpreter);
    // Tell the socket about this new interpreter
    SocketObject::newInterpreter(interpreter);
    // Tell the socket about this new interpreter
    UdpSocketObject::newInterpreter(interpreter);
    // Tell the socket server about this new interpreter
    TcpSocketServerObject::newInterpreter(interpreter);
}