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

#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include "module.h"
#include "statics.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "../commonmod/include/commonmod.h"
#include "object.h"
#include "class.h"
#include <memory>
#include <iostream>

typedef Module* (*marble_mod_init) ();

/**
* A basic cout handler that is the default handler for outputting logs to the console for modules.
*/
void CoutLogHandler(Module* module, std::string message, LOG_TYPE log_type)
{
    std::string output_msg = "";
    if(log_type == LOG_LEVEL_NOTICE)
    {
        output_msg = "NOTICE: ";
    }
    else if(log_type == LOG_LEVEL_WARNING)
    {
        output_msg = "WARNING: ";
    }
    else if(log_type == LOG_LEVEL_ERROR)
    {
        output_msg = "ERROR: ";
    }

    output_msg += message;
    std::cout << output_msg << std::endl;
}

ModuleSystem::ModuleSystem(ClassSystem* classSystem, FunctionSystem* baseFunctionSystem, LOG_HANDLER_FUNCTION log_function) : SystemHandler(SYSTEM_HANDLER_MODULE_SYSTEM, classSystem, baseFunctionSystem)
{
    if (log_function == NULL)
    {
        log_function = CoutLogHandler;
    }
    this->setLogHandler(log_function);
    addModule(new CommonModule());
}

ModuleSystem::~ModuleSystem()
{
    unloadModules();
}

void ModuleSystem::addModule(Module* module)
{
    module->setModuleSystem(this);
    module->Init();
    this->modules.push_back(module);
}

Module* ModuleSystem::loadModule(const char* filename)
{
    void* module_ptr = dlopen(filename, RTLD_NOW);
    if (!module_ptr)
        throw std::logic_error("Failed to load module: " + std::string(filename) + " " + std::string(dlerror()));

    marble_mod_init init = (marble_mod_init) dlsym(module_ptr, "marble_mod_init");
    if (dlerror())
        throw std::logic_error("Failed to find or load the marble_mod_init function in module: " + std::string(filename));
    Module* module = init();
    if (!module)
        throw std::logic_error("The module: " + std::string(filename) + " returned a NULL module upon calling its init method \"marble_mod_init\"");
    
    module->setFilename(filename);
    module->setModulePointer(module_ptr);
    addModule(module);
    return module;
}

void ModuleSystem::unloadModules()
{
    for (Module* module : this->modules)
    {
        module->unload();
        delete module;
    }
}

void ModuleSystem::tellModules(Interpreter* interpreter)
{
    for (Module* module : this->modules)
    {
        module->newInterpreter(interpreter);
    }
}

void ModuleSystem::setLogHandler(LOG_HANDLER_FUNCTION handler_func)
{
    this->log_handler = handler_func;
}

void ModuleSystem::log(Module* module, std::string message, LOG_TYPE log_type)
{
    this->log_handler(module, message, log_type);
}


std::vector<Module*> ModuleSystem::getModules()
{
    return this->modules;
}