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

#include "mathmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "mathobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

MathModule::MathModule() : Module("mathmod", "Math Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/mathmod")
{

}

MathModule::~MathModule()
{

}

void MathModule::Init()
{
    log("Math Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);

    // Register the Math class
    MathObject::registerClass(this->getModuleSystem());

    log("Math Module Initialised.", LOG_LEVEL_NOTICE);
}

void MathModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to Math. 
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Math", "Math", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Math"), {}));
}
