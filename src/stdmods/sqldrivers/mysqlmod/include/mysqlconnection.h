
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

#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H
#include "../../../../commonmod/include/commonmod_sqlconnection.h"
#include <my_global.h>
#include <mysql.h>
class MysqlConnection : public CommonModule_SqlConnection
{
public:
    MysqlConnection(Class* c);
    virtual ~MysqlConnection();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void Init(ModuleSystem* moduleSystem);

    // Native MysqlConnection functions
    static void MysqlConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MysqlConnection_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MysqlConnection_getInsertId(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);

    MYSQL* mysql_connection;
};

#endif