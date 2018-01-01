#include "module.h"
Module::Module(std::string name, MODULE_TYPE type)
{
    this->moduleSystem = NULL;
    this->name = name;
    this->type = type;
}

Module::~Module()
{

}

std::string Module::getName()
{
    return this->name;
}

MODULE_TYPE Module::getType()
{
    return this->type;
}

void Module::log(std::string message, LOG_TYPE type)
{
    this->moduleSystem->log(this, message, type);
}

void Module::setModuleSystem(ModuleSystem* moduleSystem)
{
    this->moduleSystem = moduleSystem;
}

ModuleSystem* Module::getModuleSystem()
{
    return this->moduleSystem;
}

Interpreter* Module::getInterpreter()
{
    return this->moduleSystem->getInterpreter();
}