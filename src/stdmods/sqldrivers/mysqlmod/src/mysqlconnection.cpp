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

#include "mysqlconnection.h"
#include "modulesystem.h"
MysqlConnection::MysqlConnection(Class *c) : CommonModule_SqlConnection(c)
{
    this->mysql_connection = NULL;
}

MysqlConnection::~MysqlConnection()
{
    // Did the programmer close the connection? If not then we need to do it for them
    if (this->mysql_connection != NULL)
        mysql_close(this->mysql_connection);
}

std::shared_ptr<Object> MysqlConnection::newInstance(Class *c)
{
    return std::make_shared<MysqlConnection>(c);
}

void MysqlConnection::Init(ModuleSystem *moduleSystem)
{
    /**
     * class MysqlConnection extends SQLConnection
     * 
     * This represents a connection to a MysqlServer.
     */ 
    Class *c = moduleSystem->getClassSystem()->registerClass("MysqlConnection", moduleSystem->getClassSystem()->getClassByName("SQLConnection"));
    c->setDescriptorObject(std::make_shared<MysqlConnection>(c));
    /**
     * @class MysqlConnection
     * Constructs this MysqlConnection by providing the SQLDriver that handles it
     *
     * function __construct(SQLDriver driver) : void
     */
    c->registerFunction("__construct", {VarType::fromString("SQLDriver")}, VarType::fromString("void"), MysqlConnection::MysqlConnection_Construct);

    /**
     * @class MysqlConnection
     * Closes this MYSQL connection
     * function close() : void
     */
    Function *close_function = c->registerFunction("close", {}, VarType::fromString("void"), MysqlConnection::MysqlConnection_Close);

    /**
     * @class MysqlConnection
     * Returns the last insert id to the database for this SQLConnection.
     * 
     * function getInsertId() : number
     */
    c->registerFunction("getInsertId", {}, VarType::fromString("number"), MysqlConnection::MysqlConnection_getInsertId);
}

void MysqlConnection::MysqlConnection_Construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Invoke the parent constructor of MysqlConnection.
    object->getClass("MysqlConnection")->invokeObjectParentConstructor(values, object, interpreter);
}

void MysqlConnection::MysqlConnection_Close(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Let's close the MYSQL connection
    std::shared_ptr<MysqlConnection> mysql_conn_obj = std::dynamic_pointer_cast<MysqlConnection>(object);
    mysql_close(mysql_conn_obj->mysql_connection);
    mysql_conn_obj->mysql_connection = NULL;
}

void MysqlConnection::MysqlConnection_getInsertId(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Let's get the last insert id the MYSQL connection
    std::shared_ptr<MysqlConnection> mysql_conn_obj = std::dynamic_pointer_cast<MysqlConnection>(object);
    return_value->set((double)mysql_insert_id(mysql_conn_obj->mysql_connection));
}
