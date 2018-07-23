#include "sessionmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include "sessionobject.h"
#include "sessionvaluesobject.h"
#include "filesessionobject.h"

#include <fstream>
#include <sstream>
#include <iostream>

SessionModule::SessionModule() : Module("sessionmod", "Session Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

SessionModule::~SessionModule()
{

}

void SessionModule::Init()
{
    log("Session Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    // Register the Session classes
    SessionValuesObject::registerClass(this->getModuleSystem());
    SessionObject::registerClass(this->getModuleSystem());
    FileSessionObject::registerClass(this->getModuleSystem());

    log("Session Module Initialised.", LOG_LEVEL_NOTICE);
}

void SessionModule::newInterpreter(Interpreter* interpreter)
{
   
}

