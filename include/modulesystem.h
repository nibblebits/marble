#ifndef MODULESYSTEM_H
#define MODULESYSTEM_H

#include <functional>
#include <string>

class Interpreter;
class Module;

typedef std::function<void(Module*,std::string,LOG_TYPE)> LOG_HANDLER_FUNCTION;

class ModuleSystem
{
public:
    ModuleSystem();
    virtual ~ModuleSystem();
    void setInterpreter(Interpreter* interpreter);
    Interpreter* getInterpreter();
    void loadModule(const char* filename);
    void setLogHandler(LOG_HANDLER_FUNCTION handler_func);
    void log(Module* module, std::string message, LOG_TYPE log_type);
private:
    LOG_HANDLER_FUNCTION log_handler;
    Interpreter* interpreter;

};

#endif