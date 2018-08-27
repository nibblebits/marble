#include "jsonmod.h"
#include "object.h"
#include "jsonobject.h"
#include "jsonvaluesobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

JsonModule::JsonModule() :  Module("jsonmod", "Json Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

JsonModule::~JsonModule()
{

}

void JsonModule::Init()
{
    // Register the JsobValues class
    JsonValuesObject::registerClass(this->getModuleSystem());
    // Register the Json class
    JsonObject::registerClass(this->getModuleSystem());
}

void JsonModule::newInterpreter(Interpreter *interpreter)
{
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Json", "Json", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Json"), {}));
}