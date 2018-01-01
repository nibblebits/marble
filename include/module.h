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
    virtual void Init() = 0;
private:
    MODULE_TYPE type;
    std::string name;
    ModuleSystem* moduleSystem;
};
#endif