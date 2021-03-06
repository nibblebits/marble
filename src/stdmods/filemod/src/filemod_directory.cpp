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

#include "filemod_directory.h"
#include "misc.h"
#include "filemod.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "array.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>

FileModule_Directory::FileModule_Directory(Class *c) : Object(c)
{
}

FileModule_Directory::~FileModule_Directory()
{
}

std::shared_ptr<Object> FileModule_Directory::newInstance(Class *c)
{
    return std::make_shared<Object>(c);
}

Class *FileModule_Directory::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class Directory
     * 
     * Responsible for directory related actions
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Directory");
    c->setDescriptorObject(std::make_shared<FileModule_Directory>(c));
    /**
     * @class Directory
     * 
     * function __construct() : void
     * 
     * Constructs this Directory
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), [=](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        // Blank constructor
    });

    /**
     * @class Directory
     * 
     * Creates the directory with the given string and returns weather it was succesfull or not
     * function create(string filename) : boolean
     */
    c->registerFunction("create", {VarType::fromString("string")}, VarType::fromString("boolean"), FileModule_Directory::Directory_Create);
    /**
     * @class Directory
     * 
     * Lists all files in the directory and returns them as a string array
     * function list(string path) : string[]
     */
    c->registerFunction("list", {VarType::fromString("string")}, VarType::fromString("string[]"), FileModule_Directory::Directory_List);

    /**
     * @class Directory
     * 
     * Deletes the directory with the specified filename. If the directory does not exist, is not empty or is a file or can't be deleted an IOException is thrown.
     * You require a FilePermission with write access to the directory you are trying to delete for this to succeed otherwise a PermissionException is thrown
     *
     * function delete(string filename) : void
     */
    c->registerFunction("delete", {VarType::fromString("string")}, VarType::fromString("void"), FileModule_Directory::Directory_Delete);

}

/**
 * Called when a new interpreter is running
*/
void FileModule_Directory::newInterpreter(Interpreter *interpreter)
{
    Scope *root_scope = interpreter->getRootScope();
    root_scope->createVariable("Directory", "Directory", Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Directory"), {}));
}

void FileModule_Directory::Directory_Create(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Do we have permission for this?
    std::string abs_path = getAbsolutePath(values[0].svalue);
    FilePermission::checkPermissionAllows(interpreter, caller_scope, abs_path, "w");

    // Create the directory and return weather it was ok or not
    return_value->set(mkdir(values[0].svalue.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
}

void FileModule_Directory::Directory_List(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{

    // Let's just make sure we have access to this directory
    std::string absolute_path = getAbsolutePath(values[0].svalue);
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_path.c_str(), "r");

    DIR *dir;
    struct dirent *ent;
    std::vector<std::string> entries;
    if ((dir = opendir(values[0].svalue.c_str())) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {
            entries.push_back(ent->d_name);
        }
        closedir(dir);
    }
    else
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to list directories in location: \"" + values[0].svalue + "\"", interpreter->getStackTraceLog());
    }

    if (entries.size() > 0)
    {

        Variable* variables = new Variable[entries.size()];
        for (int i = 0; i < entries.size(); i++)
        {
            Variable* var = &variables[i];
            var->type = VARIABLE_TYPE_STRING;
            var->value.type = VALUE_TYPE_STRING;
            var->value.holder = var;
            var->value.svalue = entries[i];
        }
        return_value->set(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, entries.size()));
    }
}


void FileModule_Directory::Directory_Delete(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Let's just make sure we have write access to this directory
    std::string absolute_path = getAbsolutePath(values[0].svalue);
    FilePermission::checkPermissionAllows(interpreter, caller_scope, absolute_path.c_str(), "w");
    if(rmdir(absolute_path.c_str()) != 0)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to delete the directory: " + absolute_path + " is it empty?", interpreter->getStackTraceLog());

}