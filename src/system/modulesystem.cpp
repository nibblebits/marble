#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include "module.h"
#include "statics.h"
#include "modulesystem.h"
#include "interpreter.h"

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

ModuleSystem::ModuleSystem() : SystemHandler(SYSTEM_HANDLER_MODULE_SYSTEM, NULL, NULL)
{
    this->setLogHandler(CoutLogHandler);
    activate();
    deactivate();
}

ModuleSystem::~ModuleSystem()
{

}

void ModuleSystem::loadModule(const char* filename)
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
    module->setModuleSystem(this);
    module->Init();
}

void ModuleSystem::setLogHandler(LOG_HANDLER_FUNCTION handler_func)
{
    this->log_handler = handler_func;
}

void ModuleSystem::log(Module* module, std::string message, LOG_TYPE log_type)
{
    this->log_handler(module, message, log_type);
}
