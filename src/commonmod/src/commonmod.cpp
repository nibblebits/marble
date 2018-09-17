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

#include "commonmod.h"
#include "object.h"
#include "commonmod_sqldriver.h"
#include "commonmod_sqlstatement.h"
#include "commonmod_sqlconnection.h"
#include "commonmod_sqlresult.h"
#include "commonmod_sqlrecord.h"
#include "commonmod_preparedstatement.h"
#include "commonmod_system.h"
#include "commonmod_value.h"
#include "commonmod_list.h"
#include "commonmod_vector.h"
#include "commonmod_map.h"

#include <sstream>
#include <iostream>
#include <time.h>
#include <stdio.h>

CommonModule::CommonModule() : Module("commonmod", "Common Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/commonmod")
{

}

CommonModule::~CommonModule()
{

}

void CommonModule::Init()
{
    log("Common Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    // REGISTER EXCEPTIONS
    Class* exception_cls = this->getModuleSystem()->getClassSystem()->getClassByName("Exception");
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("EndOfStreamException", exception_cls);
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        object->getClass()->invokeObjectParentConstructor(arguments, object, interpreter);
    });
    // END OF EXCEPTIONS

    CommonModule_OutputStream::registerClass(this->getModuleSystem());
    CommonModule_InputStream::registerClass(this->getModuleSystem());
    CommonModule_StringUtils::registerClass(this->getModuleSystem());
    CommonModule_SqlDriver::registerClass(this->getModuleSystem());
    CommonModule_SqlStatement::registerClass(this->getModuleSystem());
    CommonModule_PreparedStatement::registerClass(this->getModuleSystem());
    CommonModule_SqlConnection::registerClass(this->getModuleSystem());
    CommonModule_SqlResult::registerClass(this->getModuleSystem());
    CommonModule_SqlRecord::registerClass(this->getModuleSystem());
    CommonModule_System::registerClass(this->getModuleSystem());
    CommonModule_Value::registerClass(this->getModuleSystem());
    CommonModule_List::registerClass(this->getModuleSystem());
    CommonModule_Vector::registerClass(this->getModuleSystem());
    CommonModule_Map::registerClass(this->getModuleSystem());

    log("Common Module Initialised.", LOG_LEVEL_NOTICE);
}

void CommonModule::newInterpreter(Interpreter* interpreter)
{
    // Let's create a variable named StringUtils that points to an object of class type StringUtils
    Class* c = this->getModuleSystem()->getClassSystem()->getClassByName("StringUtils");
    interpreter->getCurrentScope()->createVariable("StringUtils", "StringUtils", Object::create(c));

    // Let's let the SqlDriver know we have a new interpreter.
    CommonModule_SqlDriver::newInterpreter(interpreter);

    // Let's let the System know we have a new interpreter
    CommonModule_System::newInterpreter(interpreter);
}
