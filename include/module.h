#ifndef MODULE_H
#define MODULE_H
#include <string>
#include "statics.h"
#include "interpreter.h"
#include "modulesystem.h"
class Module
{
public:
    Module(std::string name, MODULE_TYPE type);
    virtual ~Module();
    std::string getName();
    MODULE_TYPE getType();
    void log(std::string message, LOG_TYPE type);
    void setModuleSystem(ModuleSystem* moduleSystem);
    ModuleSystem* getModuleSystem();
    /**
     * The newInterpreter method is called when a new Interpreter is created that may or may not have access to this module in the future.
     * Here you can register any global variables or anything special that only this Interpreter should know about.
     */
    virtual void newInterpreter(Interpreter* interpreter) = 0;
    virtual void Init() = 0;
private:
    MODULE_TYPE type;
    std::string name;
    ModuleSystem* moduleSystem;
};
#endif