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


FileModule::FileModule() : Module("filemod", "File Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/filemod")
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
