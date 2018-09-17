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

#include "sessionmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include "sessionobject.h"
#include "sessionvaluesobject.h"
#include "filesessionobject.h"

#include <fstream>
#include <sstream>
#include <iostream>

SessionModule::SessionModule() : Module("sessionmod", "Session Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/sessionmod")
{

}

SessionModule::~SessionModule()
{

}

void SessionModule::Init()
{
    // Register the Session classes
    SessionValuesObject::registerClass(this->getModuleSystem());
    SessionObject::registerClass(this->getModuleSystem());
    FileSessionObject::registerClass(this->getModuleSystem());
}

void SessionModule::newInterpreter(Interpreter* interpreter)
{
   
}

