#include "module.h"
Module::Module(std::string name, std::string module_descriptor, MODULE_TYPE type)
{
    this->moduleSystem = NULL;
    this->descriptor = module_descriptor;
    this->name = name;
    this->type = type;
    this->filename = "";
    this->module_ptr = NULL;
}

Module::~Module()
{

}

std::string Module::getName()
{
    return this->name;
}

std::string Module::getDescriptor()
{
    return this->descriptor;
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

void Module::setFilename(std::string filename)
{
    this->filename = filename;
}

std::string Module::getFilename()
{
    return this->filename;
}

void Module::setModulePointer(void* module_ptr)
{
    this->module_ptr = module_ptr;
}
    
void* Module::getModulePointer()
{
    return this->module_ptr;
}

ModuleSystem* Module::getModuleSystem()
{
    return this->moduleSystem;
}
