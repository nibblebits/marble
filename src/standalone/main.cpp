#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <memory>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "interpreter.h"
#include "basesystemhandler.h"
#include "object.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptionobject.h"
#include "misc.h"
#include "module.h"
#include "exceptions/systemexception.h"
#include "../stdmods/iomod/include/iopermission.h"

BaseSystemHandler *baseHandler = NULL;
ModuleSystem *moduleSystem = NULL;
std::shared_ptr<PermissionsObject> set_permissions;
std::string configFileName = "./config.marble";
bool loadConfiguration()
{
    Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
    interpreter.setOutputFunction([](const char *data, int length) {
        for (int i = 0; i < length; i++)
        {
            std::cout << (char)data[i];
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

void ModuleLogHandler(Module *module, std::string message, LOG_TYPE log_type)
{
    if (log_type == LOG_LEVEL_ERROR)
        std::cout << message << std::endl;
}

void interpret(std::string filename)
{
    Interpreter interpreter(moduleSystem->getClassSystem(), moduleSystem->getFunctionSystem());
    interpreter.setTimeout(0);
    interpreter.setOutputFunction([](const char *data, int length) {
        for (int i = 0; i < length; i++)
        {
            std::cout << (char)data[i];
        }
    });

    interpreter.setInputFunction([]() -> std::string {
        std::string s;
        std::cin >> s;
        return s;
    });

    Logger *logger = interpreter.getLogger();
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

void show_version_information()
{
    std::cout << "Marble Codename: " << Interpreter::getCodename() << std::endl;
    std::cout << "Marble version: " << Interpreter::getVersion() << std::endl;

    baseHandler = new BaseSystemHandler();
    moduleSystem = new ModuleSystem(baseHandler->getClassSystem(), baseHandler->getFunctionSystem(), ModuleLogHandler);
    // Load the configuration and return on failure
    if (!loadConfiguration())
        throw std::logic_error("Failed to load configuration file" + configFileName + " which is required to get module information");
    
    std::cout << std::endl;
    std::cout << "Loaded marble modules based on the configuration file: " + configFileName << std::endl;
    std::cout << "----------------------" << std::endl;
    for (Module* module : moduleSystem->getModules())
    {
        std::cout << "Module: " << module->getDescriptor() <<  ":" << module->getName() << ", Version: " << module->getVersion() << " path:" << module->getFilename() << " help url: " << module->getHelpPath() << std::endl;
    }
    std::cout << "----------------------" << std::endl;

    delete baseHandler;
    delete moduleSystem;
}

void show_mod_information(int argc, char **argv)
{
    if (argc < 3)
        throw std::logic_error("You must provide the modules file path to get module information");
}
int begin(int argc, char **argv)
{
    if (argc < 2)
        throw std::logic_error("You must provide a filename");

    if (strcmp(argv[1], "-v") == 0)
    {
        show_version_information();
        return 0;
    }
    else if (strcmp(argv[1], "-mod"))
    {
        show_mod_information(argc, argv);
    }

    // Let's load the configuration and start interpreting
    baseHandler = new BaseSystemHandler();
    moduleSystem = new ModuleSystem(baseHandler->getClassSystem(), baseHandler->getFunctionSystem(), ModuleLogHandler);
    // Load the configuration and return on failure
    if (!loadConfiguration())
        return 1;

    interpret(argv[1]);

    delete baseHandler;
    delete moduleSystem;

    return 0;
}

int main(int argc, char **argv)
{
    try
    {
        return begin(argc, argv);
    }
    catch (std::logic_error &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}
