#include "networkmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include "internetprotocolobject.h"
#include "ipv4addressobject.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

NetworkModule::NetworkModule() : Module("networkmod", "Network Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

NetworkModule::~NetworkModule()
{

}

void NetworkModule::Init()
{

   // Register the Ipv4Address class
   Ipv4AddressObject::registerClass(this->getModuleSystem());

   // Register the InternetProtocol class
   InternetProtocolObject::registerClass(this->getModuleSystem());
}

void NetworkModule::newInterpreter(Interpreter* interpreter)
{
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("InternetProtocol", "InternetProtocol", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("InternetProtocol"), {}));
}