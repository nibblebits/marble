/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "module.h"
Module::Module(std::string name, std::string module_descriptor, MODULE_TYPE type, std::string version, std::string help_path)
{
    this->moduleSystem = NULL;
    this->descriptor = module_descriptor;
    this->name = name;
    this->type = type;
    this->version = version;
    this->help_path = help_path;
    this->filename = "";
    this->module_ptr = NULL;
}

Module::~Module()
{

}

void Module::unload()
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

std::string Module::getVersion()
{
    return this->version;
}

std::string Module::getHelpPath()
{
    return this->help_path;
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
