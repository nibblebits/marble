#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include "module.h"
#include "modulesystem.h"
#include "interpreter.h"

typedef Module* (*marble_mod_init) (Interpreter* interpreter);

ModuleSystem::ModuleSystem()
{
    this->interpreter = NULL;
}

ModuleSystem::~ModuleSystem()
{

}

void ModuleSystem::setInterpreter(Interpreter* interpreter)
{
    this->interpreter = interpreter;
}

void ModuleSystem::loadModule(const char* filename)
{
    if (!this->interpreter)
        throw std::logic_error("You must set this ModuleSystem to an interpreter before you can load modules");
        
    void* module_ptr = dlopen(filename, RTLD_NOW);
    if (!module_ptr)
        throw std::logic_error("Failed to load module: " + std::string(filename) + " " + std::string(dlerror()));

    marble_mod_init init = (marble_mod_init) dlsym(module_ptr, "marble_mod_init");
    if (dlerror())
        throw std::logic_error("Failed to find or load the marble_mod_init function in module: " + std::string(filename));
    Module* module = init(this->interpreter);
    if (!module)
        throw std::logic_error("The module: " + std::string(filename) + " returned a NULL module upon calling its init method \"marble_mod_init\"");
    module->Init();
}