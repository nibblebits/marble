#include "commonmod.h"
#include "object.h"
#include "function.h"
#include <sstream>
#include <time.h>
#include <stdio.h>

CommonModule_OutputStream::CommonModule_OutputStream(Class* c) : Object(c)
{
  
}

CommonModule_OutputStream::~CommonModule_OutputStream()
{

}

std::shared_ptr<Object> CommonModule_OutputStream::newInstance(Class* c)
{
    return std::make_shared<CommonModule_OutputStream>(c);
}

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
    
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("OutputStream");
    c->is_pure = true;
    c->setDescriptorObject(std::make_shared<CommonModule_OutputStream>(c));
    Function* f = c->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
    });
    f->is_pure = true;

    log("File Module Initialised.", LOG_LEVEL_NOTICE);
}

void CommonModule::newInterpreter(Interpreter* interpreter)
{
  
}

// Native IO functions/methods
