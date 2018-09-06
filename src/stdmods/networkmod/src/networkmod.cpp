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
#include "networkpermission.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <curl/curl.h>
#include <unistd.h>

NetworkModule::NetworkModule() : Module("networkmod", "Network Module", MODULE_TYPE_MARBLE_LIBRARY)
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
    data_ptr->erase(data_ptr->begin(), data_ptr->begin()+amount_to_read);

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
}

void NetworkModule::newInterpreter(Interpreter *interpreter)
{
    Scope *root_scope = interpreter->getRootScope();
    root_scope->createVariable("InternetProtocol", "InternetProtocol", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("InternetProtocol"), {}));

    // Tell the CurlObject about this new interpreter
    CurlObject::newInterpreter(interpreter);
}