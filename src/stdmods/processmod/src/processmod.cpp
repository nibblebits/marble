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

#include "processmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "shellobject.h"
#include "shellpermission.h"
#include "shellexecutionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

ProcessModule::ProcessModule() :  Module("processmod", "Process Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/processmod")
{

}

ProcessModule::~ProcessModule()
{

}

void ProcessModule::Init()
{

    // Register the ShellPermission class
    ShellPermission::registerClass(this->getModuleSystem());

    // Register the ShellExecutionObject
    ShellExecutionObject::registerClass(this->getModuleSystem());

    // Register the ShellObject
    ShellObject::registerClass(this->getModuleSystem());
}

void ProcessModule::newInterpreter(Interpreter *interpreter)
{

}