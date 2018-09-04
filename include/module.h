#ifndef MODULE_H
#define MODULE_H
#include <string>
#include "statics.h"
#include "interpreter.h"
#include "modulesystem.h"
class Module
{
public:
    Module(std::string name, std::string module_descriptor, MODULE_TYPE type);
    virtual ~Module();
    std::string getName();
    std::string getDescriptor();
    MODULE_TYPE getType();
    void log(std::string message, LOG_TYPE type);
    void setModuleSystem(ModuleSystem* moduleSystem);
    void setFilename(std::string filename);
    std::string getFilename();
    void setModulePointer(void* module_ptr);
    void* getModulePointer();
    ModuleSystem* getModuleSystem();
    /**
     * The newInterpreter method is called when a new Interpreter is created that may or may not have access to this module in the future.
     * Here you can register any global variables or anything special that only this Interpreter should know about.
     */
    virtual void newInterpreter(Interpreter* interpreter) = 0;
    virtual void Init() = 0;
    virtual void unload();
private:
    MODULE_TYPE type;
    std::string descriptor;
    std::string name;
    std::string filename;
    // The pointer to the library loaded with dlopen
    void* module_ptr;
    ModuleSystem* moduleSystem;
};
#endif