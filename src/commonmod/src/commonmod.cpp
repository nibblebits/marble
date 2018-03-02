#include "commonmod.h"
#include "object.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include <stdio.h>

CommonModule::CommonModule() : Module("commonmod", "Common Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

CommonModule::~CommonModule()
{

}

void CommonModule::Init()
{
    log("Common Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    CommonModule_OutputStream::registerClass(this->getModuleSystem());

    log("Common Module Initialised.", LOG_LEVEL_NOTICE);

    log("String Utils Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    CommonModule_StringUtils::registerClass(this->getModuleSystem());

    log("String Utils Initialised.", LOG_LEVEL_NOTICE);

}

void CommonModule::newInterpreter(Interpreter* interpreter)
{
    Class* c = this->getModuleSystem()->getClassSystem()->getClassByName("StringUtils");
    interpreter->getCurrentScope()->createVariable("StringUtils", "StringUtils", std::make_shared<CommonModule_StringUtils>(c));
}

// Native IO functions/methods
