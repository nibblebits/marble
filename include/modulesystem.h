#ifndef MODULESYSTEM_H
#define MODULESYSTEM_H

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "systemhandler.h"

class Interpreter;
class Module;
class ClassSystem;
class FunctionSystem;

typedef std::function<void(Module*,std::string,LOG_TYPE)> LOG_HANDLER_FUNCTION;

/**
 * 
 * The ModuleSystem loads and handles all modules for interpreter's to use.
 * You should not load modules after interpreter's start to use this ModuleSystem even if they have finished using it.
 * Loading of modules should be done before any interpreting happens.
 */ 
class ModuleSystem : public SystemHandler
{
public:
    ModuleSystem(ClassSystem* classSystem, FunctionSystem* baseFunctionSystem, LOG_HANDLER_FUNCTION log_function=NULL);
    virtual ~ModuleSystem();
    /**
     * Adds the module to this ModuleSystem. A module should only be added to one ModuleSystem to work correctly.
     */
    void addModule(Module* module);
    Module* loadModule(const char* filename);
    void setLogHandler(LOG_HANDLER_FUNCTION handler_func);
    void log(Module* module, std::string message, LOG_TYPE log_type);
    /**
     * Tell's all the modules about this newly existing Interpreter.
     */
    void tellModules(Interpreter* interpreter);

    /**
     * Unloads the modules apart of this ModuleSystem. If the same module is in multiple ModuleSystems it won't be a good idea to call this
     */
    void unloadModules();

    /**
     * Returns the  modules loaded into this ModuleSystem
     */
    std::vector<Module*> getModules();
private:
    std::vector<Module*> modules;
    LOG_HANDLER_FUNCTION log_handler;

};

#endif