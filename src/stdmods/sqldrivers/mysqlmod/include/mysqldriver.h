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

#ifndef MYSQLDRIVER_H
#define MYSQLDRIVER_H

#include "../../../../commonmod/include/commonmod_sqldriver.h"
class ModuleSystem;
class Interpreter;
class MysqlDriver : public CommonModule_SqlDriver
{
    public:
        MysqlDriver(Class* c);
        virtual ~MysqlDriver();
        virtual std::shared_ptr<Object> newInstance(Class* c);

        static void Init(ModuleSystem* moduleSystem);

        // Native MysqlDriver functions
        static void MysqlDriver_Execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        static void MysqlDriver_Connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        static void MysqlDriver_Escape(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif