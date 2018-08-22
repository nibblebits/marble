#include "networkmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include "internetprotocolobject.h"
#include "ipv4addressobject.h"
#include "curlobject.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <curl/curl.h>

NetworkModule::NetworkModule() : Module("networkmod", "Network Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

NetworkModule::~NetworkModule()
{

}

size_t NetworkModule::CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void NetworkModule::Init()
{

   // Let's setup CURL for use in other objects in the NetworkModule
   curl_global_init(CURL_GLOBAL_ALL);

   // Register the Ipv4Address class
   Ipv4AddressObject::registerClass(this->getModuleSystem());

   // Register the InternetProtocol class
   InternetProtocolObject::registerClass(this->getModuleSystem());

   // Register the Curl class
   CurlObject::registerClass(this->getModuleSystem());

}

void NetworkModule::newInterpreter(Interpreter* interpreter)
{
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("InternetProtocol", "InternetProtocol", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("InternetProtocol"), {}));

    // Tell the CurlObject about this new interpreter
    CurlObject::newInterpreter(interpreter);
}