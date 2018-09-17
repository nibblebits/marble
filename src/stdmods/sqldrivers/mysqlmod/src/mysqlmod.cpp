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

#include "mysqlmod.h"
#include "mysqlconnection.h"
#include "mysqldriver.h"

MysqlModule::MysqlModule() : Module("mysqlmod", "Mysql Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "https://marble.wiki/api/module/mysqlmod")
{

}

MysqlModule::~MysqlModule()
{

}

void MysqlModule::Init()
{
    // Let's setup
    MysqlConnection::Init(this->getModuleSystem());
    MysqlDriver::Init(this->getModuleSystem());
}

void MysqlModule::newInterpreter(Interpreter* interpreter)
{
    // Register the MYSQLDriver in the interpreter instance
    std::shared_ptr<MysqlDriver> mysql_driver 
                        = std::dynamic_pointer_cast<MysqlDriver>(Object::create(interpreter->getClassSystem()->getClassByName("MysqlDriver")));
    interpreter->registerSQLDriver(mysql_driver);
}


void MysqlModule::unload()
{
    // Shutdown the mysql library
    mysql_library_end();
}