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

    log("File Module Initialised.", LOG_LEVEL_NOTICE);
}

void CommonModule::newInterpreter(Interpreter* interpreter)
{
  
}

// Native IO functions/methods
