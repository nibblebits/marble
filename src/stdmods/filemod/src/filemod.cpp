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


FileModule::FileModule() : Module("filemod", "File Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "ADD_HELP_PATH_HERE")
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
