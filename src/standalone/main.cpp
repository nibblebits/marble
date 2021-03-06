/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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
std::string configFileName = "/etc/marble/config.marble";
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

void interpret(std::string filename, std::vector<std::string> arguments)
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
        std::getline(std::cin, s);
        return s;
    });

    // Set our argv in the Interpreter so it can be resolved.
    interpreter.setArgv(arguments);

    Logger *logger = interpreter.getLogger();
    interpreter.setModuleSystem(moduleSystem);
    interpreter.getGlobalScope()->permissions = set_permissions;
    // Let's setup the session key
    interpreter.properties["session_key"] = "simpleclientsessionkey";

    try
    {
        interpreter.runScript(filename.c_str());
    }
    catch (std::logic_error &ex)
    {
        std::string error_msg = "";
        if (logger->hasErrors())
        {
            for (LogEntry entry : logger->entries)
            {
                error_msg +=  entry.message + " on line: " + std::to_string(entry.posInfo.line) + ", col: " + std::to_string(entry.posInfo.col) + " file: " + entry.posInfo.filename + "\n";
            }
        }
        else
        {
            error_msg = ex.what();
        }

        throw std::logic_error(error_msg);
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
    for (Module *module : moduleSystem->getModules())
    {
        std::cout << "Module: " << module->getDescriptor() << ":" << module->getName() << ", Version: " << module->getVersion() << " path:" << module->getFilename() << " help url: " << module->getHelpPath() << std::endl;
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

void show_help()
{
    std::cout << "Marble Codename: " << Interpreter::getCodename() << std::endl;
    std::cout << "Marble version: " << Interpreter::getVersion() << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Use \"marble /my/path/filename.marble\" if you wish to run a marble file with the default configuration" << std::endl;
    std::cout << "Use -setconfig to change the configuration file for when running a marble script \"marble -setconfig /path/to/config.marble /file/to/run.marble\"" << std::endl;
    std::cout << "Append the argument -v to see versioning information of this marble interpreter and its modules: \"marble -v\"" << std::endl;
    std::cout << "---------------------" << std::endl;
}
int begin(int argc, char **argv)
{
    if (argc < 2)
    {
        show_help();
        return 0;
    }

    std::string fileToInterpret = argv[1];
    int start_of_marble_args = 1;
    if (strcmp(argv[1], "-v") == 0)
    {
        show_version_information();
        return 0;
    }
    else if (strcmp(argv[1], "-setconfig") == 0)
    {
        if (argc < 3)
            throw std::logic_error("You used -setconfig but you did not specify a path to the configuration file to use");

        if (argc < 4)
            throw std::logic_error("You must provide a filename to use this new configuration with");

        configFileName = argv[2];
        fileToInterpret = argv[3];
        start_of_marble_args = 3;
    }

    std::vector<std::string> arguments;
    for (int i = start_of_marble_args; i < argc; i++)
    {
        arguments.push_back(argv[i]);
    }
    // Let's load the configuration and start interpreting
    baseHandler = new BaseSystemHandler();
    moduleSystem = new ModuleSystem(baseHandler->getClassSystem(), baseHandler->getFunctionSystem(), ModuleLogHandler);
    // Load the configuration and return on failure
    if (!loadConfiguration())
        return 1;

    interpret(fileToInterpret, arguments);

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
