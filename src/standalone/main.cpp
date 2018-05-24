#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <memory>
#include "interpreter.h"
#include "basesystemhandler.h"
#include "object.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "../stdmods/iomod/include/iopermission.h"

BaseSystemHandler* baseHandler = NULL;
ModuleSystem* moduleSystem = NULL;
std::shared_ptr<PermissionsObject> set_permissions;
std::string configFileName = "./config.marble";
bool loadConfiguration()
{
    std::cout << "LOADING MARBLE CONFIGURATION: " << configFileName << std::endl;
    Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
    interpreter.setOutputFunction([](const char* data) {
        std::cout << data;
    });
    // Configurations should not be bound to permissions.
    interpreter.setNoPermissionRestrictions(true);
    interpreter.setModuleSystem(moduleSystem);
    try
    {
        interpreter.runScript(configFileName.c_str());
    }
    catch(...)
    {
        std::cout << "Failed to load or run the configuration script: " << configFileName << std::endl;
        throw;
    }

    /* The set permissions become equal to the permissions of the configuration interpreter
       Do be aware this type of setup only works in a single threaded environment in a multithreaded environment the permission object must be cloned
       when inserting it into interpreters that must be run later. */ 
    set_permissions = interpreter.getRootScope()->permissions;

    return true;
}
void interpret()
{
    Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
    interpreter.setOutputFunction([](const char* data) {
        std::cout << data;
    });
    
    interpreter.setInputFunction([]()->std::string {
        std::string s;
        std::cin >> s;
        return s;
    });

    Logger* logger = interpreter.getLogger();
    interpreter.setModuleSystem(moduleSystem);
    interpreter.getRootScope()->permissions = set_permissions;
    interpreter.runScript("./test.marble");
    for (LogEntry entry : logger->entries)
    {
        std::cout << entry.message << " on line: " << entry.posInfo.line << ", col: " << entry.posInfo.col << std::endl;
    }    
}
int main(int argc, char** argv)
{
    baseHandler = new BaseSystemHandler();
    moduleSystem = new ModuleSystem(baseHandler->getClassSystem(), baseHandler->getFunctionSystem());
   // Load the configuration and return on failure
    if(!loadConfiguration())
       return 1;

    interpret();

    delete baseHandler;
    delete moduleSystem;
    std::cout << "Program terminated" << std::endl;
    return 0;
}
