#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <memory>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include "interpreter.h"
#include "basesystemhandler.h"
#include "object.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptionobject.h"
#include "misc.h"
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
    interpreter.setOutputFunction([](const char* data, int length) {
        for (int i = 0; i < length; i++) {
            std::cout << (char) data[i];
        }
    });
    // Configurations should not be bound to permissions.
    interpreter.setNoPermissionRestrictions(true);
    interpreter.setModuleSystem(moduleSystem);

    interpreter.runScript(configFileName.c_str());


    /* The set permissions become equal to the permissions of the configuration interpreter
       Do be aware this type of setup only works in a single threaded environment in a multithreaded environment the permission object must be cloned
       when inserting it into interpreters that must be run later. */ 
    set_permissions = interpreter.getGlobalScope()->permissions;

    return true;
}
void interpret(std::string filename)
{
        Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
        interpreter.setTimeout(5000);
        interpreter.setOutputFunction([](const char* data, int length) {
            for (int i = 0; i < length; i++) {
                std::cout << (char) data[i];
            }
        });
        
        interpreter.setInputFunction([]()->std::string {
            std::string s;
            std::cin >> s;
            return s;
        });

        Logger* logger = interpreter.getLogger();
        interpreter.setModuleSystem(moduleSystem);
        interpreter.getGlobalScope()->permissions = set_permissions;
        // Let's setup the session key
        interpreter.properties["session_key"] = "simpleclientsessionkey";

        interpreter.runScript(filename.c_str());
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

    chdir("./marblelab");
    if (argc < 2)
        throw std::logic_error("You must provide a filename");
    interpret(argv[1]);
    
    delete baseHandler;
    delete moduleSystem;
    return 0;
}
