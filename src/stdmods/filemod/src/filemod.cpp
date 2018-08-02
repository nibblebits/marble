#include "filemod.h"
#include "object.h"
#include "function.h"
#include "filepermission.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "misc.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include <stdio.h>


std::shared_ptr<Object> FileModule_File::newInstance(Class* c)
{
    return std::make_shared<FileModule_File>(c);
}

FileModule::FileModule() : Module("filemod", "File Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

FileModule::~FileModule()
{

}

void FileModule::Init()
{
    log("File Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    ClassSystem* c_system = this->getModuleSystem()->getClassSystem();

    // Register the FileOutputStream class
    FileModule_FileOutputStream::registerClass(this->getModuleSystem());

    // Register the FileInputStream class
    FileModule_FileInputStream::registerClass(this->getModuleSystem());

    // Register the File class
    FileModule_File::registerClass(this->getModuleSystem());

    // Register the Directory class
    FileModule_Directory::registerClass(this->getModuleSystem());


    log("File Module Initialised.", LOG_LEVEL_NOTICE);
}

void FileModule::newInterpreter(Interpreter* interpreter)
{
    // Let the File class know about this new interpreter
    FileModule_File::newInterpreter(interpreter);

    // Let the Directory class know about this new interpreter
    FileModule_Directory::newInterpreter(interpreter);
}


bool FileModule::isWriteMode(std::string mode)
{
    return mode == "w" || mode == "a";
}

bool FileModule::isReadMode(std::string mode)
{
    return mode == "r";
}

void FileModule::permissionCheck(Interpreter* interpreter, Scope* caller_scope, std::string absolute_filename_path, std::string mode)
{
    // Let's ensure this scope has permission to open this file
    if (!interpreter->hasNoPermissionRestrictions())
    {
        // We don't have access yet let's see how this turns out
        bool has_access = false;

        std::vector<std::shared_ptr<PermissionObject>> permission_list = caller_scope->getPermissionList("FilePermission");
        // If the permission list is empty then we don't have permission to open this file
        if (permission_list.empty())
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You don't have permission to open the file: " + absolute_filename_path);
        }
        for (std::shared_ptr<PermissionObject> perm : permission_list)
        {
            std::shared_ptr<FilePermission> permission = std::dynamic_pointer_cast<FilePermission>(perm);
            // Do we have access to the directory we are trying to access?
            if (startsWith(absolute_filename_path, permission->location->value.svalue))
            {
                if (FileModule::isReadMode(mode))
                {
                    // If we can't read then just continue and hopefully we will find a permission that allows this
                    if (!permission->can_read->value.dvalue)
                    {
                        continue;
                    }
                }
                else if (FileModule::isWriteMode(mode))
                {
                    // If we can't write then just continue and hopefully we will find a permission that allows this
                    if (!permission->can_write->value.dvalue)
                    {
                        continue;
                    }
                }
                has_access = true;
                break;
            }
        }

        if (!has_access)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You don't have the permission to access the path: " + absolute_filename_path + " for mode: " + mode);
        }
    }
}
