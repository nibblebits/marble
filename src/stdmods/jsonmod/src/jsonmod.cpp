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

#include "jsonmod.h"
#include "object.h"
#include "jsonobject.h"
#include "jsonvaluesobject.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

JsonModule::JsonModule() :  Module("jsonmod", "Json Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/jsonmod")
{

}

JsonModule::~JsonModule()
{

}

void JsonModule::Init()
{
    // Register the JsobValues class
    JsonValuesObject::registerClass(this->getModuleSystem());
    // Register the Json class
    JsonObject::registerClass(this->getModuleSystem());
}

void JsonModule::newInterpreter(Interpreter *interpreter)
{
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Json", "Json", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Json"), {}));
}