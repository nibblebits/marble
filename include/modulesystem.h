#ifndef MODULESYSTEM_H
#define MODULESYSTEM_H

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "systemhandler.h"

class Interpreter;
class Module;

typedef std::function<void(Module*,std::string,LOG_TYPE)> LOG_HANDLER_FUNCTION;

/**
 * 
 * The ModuleSystem loads and handles all modules for interpreter's to use.
 * You should not load modules after interpreter's start to use this ModuleSystem even if they have finished using it.
 * Loading of modules should be done before any interpreting happens.
 */ 
class ModuleSystem
{
public:
    ModuleSystem();
    virtual ~ModuleSystem();
    void loadModule(const char* filename);
    void setLogHandler(LOG_HANDLER_FUNCTION handler_func);
    void log(Module* module, std::string message, LOG_TYPE log_type);
    /**
     * Tell's all the modules about this newly existing Interpreter.
     */
    void tellModules(Interpreter* interpreter);
    FunctionSystem* getFunctionSystem();
    ClassSystem* getClassSystem();
private:
    std::vector<Module*> modules;
    std::unique_ptr<ClassSystem> classSystem;
    std::unique_ptr<FunctionSystem> functionSystem;
    LOG_HANDLER_FUNCTION log_handler;

};

#endif