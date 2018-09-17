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

#ifndef MODULE_H
#define MODULE_H
#include <string>
#include "statics.h"
#include "interpreter.h"
#include "modulesystem.h"
class Module
{
public:
    Module(std::string name, std::string module_descriptor, MODULE_TYPE type, std::string version, std::string help_path);
    virtual ~Module();
    std::string getName();
    std::string getDescriptor();
    MODULE_TYPE getType();
    void log(std::string message, LOG_TYPE type);
    void setModuleSystem(ModuleSystem* moduleSystem);
    void setFilename(std::string filename);
    std::string getFilename();
    std::string getVersion();
    std::string getHelpPath();
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
    std::string version;
    std::string help_path;
    // The pointer to the library loaded with dlopen
    void* module_ptr;
    ModuleSystem* moduleSystem;
};
#endif