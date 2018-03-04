#include "filemod.h"
#include "object.h"
#include "function.h"
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
    // Register the  FileOutputStream class
    FileModule_FileOutputStream::registerClass(this->getModuleSystem());

    // Register the File class
    FileModule_File::registerClass(this->getModuleSystem());

    log("File Module Initialised.", LOG_LEVEL_NOTICE);
}

void FileModule::newInterpreter(Interpreter* interpreter)
{
  
}