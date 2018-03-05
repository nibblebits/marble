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
    
    // REGISTER EXCEPTIONS
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("EndOfStreamException");
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
    });
    // END OF EXCEPTIONS

    CommonModule_OutputStream::registerClass(this->getModuleSystem());
    CommonModule_InputStream::registerClass(this->getModuleSystem());
    CommonModule_StringUtils::registerClass(this->getModuleSystem());

    log("Common Module Initialised.", LOG_LEVEL_NOTICE);
}

void CommonModule::newInterpreter(Interpreter* interpreter)
{
    Class* c = this->getModuleSystem()->getClassSystem()->getClassByName("StringUtils");
    interpreter->getCurrentScope()->createVariable("StringUtils", "StringUtils", std::make_shared<CommonModule_StringUtils>(c));
}

// Native IO functions/methods
