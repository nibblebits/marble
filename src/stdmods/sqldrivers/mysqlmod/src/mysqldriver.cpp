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

#include "mysqldriver.h"
#include "modulesystem.h"
#include "function.h"
#include "interpreter.h"
#include "mysqlconnection.h"
#include "../../../../commonmod/include/commonmod_sqlrecord.h"
#include "../../../../commonmod/include/commonmod_sqlresult.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <my_global.h>
#include <mysql.h>
#include <iostream>

MysqlDriver::MysqlDriver(Class* c) : CommonModule_SqlDriver(c)
{

}

MysqlDriver::~MysqlDriver()
{

}

void MysqlDriver::Init(ModuleSystem* moduleSystem)
{
    /**
     * class MysqlDriver extends SQLDriver
     * 
     * This is the MysqlDriver it can be used to connect to a MysqlServer and execute statements
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("MysqlDriver", moduleSystem->getClassSystem()->getClassByName("SQLDriver"));
    c->setDescriptorObject(std::make_shared<MysqlDriver>(c));
    /**
     * @class MysqlDriver
     * 
     * Constructs this MysqlDriver
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /*
    * @class MysqlDriver
    * Connects to the provided MysqlServer based on the host, username, password and database provided.
    * Once connected to the database returns a SQLConnection representing the connection to this database
    * 
    * function connect(string host, string username, string password, string database) : SQLConnection
    */
    Function* connect_function = c->registerFunction("connect", {
                                    VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string")},
                                    VarType::fromString("SQLConnection"), MysqlDriver::MysqlDriver_Connect);

    /**
     * @class MysqlDriver
     * 
     * Executes the given query for the connection and statement. The statement also holds the query.
     * This function can be ignored if you are not implementing a SQLDriver the reason for this is because it is automatically called when
     * executing a SQLStatement
     * 
     * function execute(SQLConnection connection, SQLStatement statement, string query) : SQLResult
     */
    Function* execute = c->registerFunction("execute", {VarType::fromString("SQLConnection"), VarType::fromString("SQLStatement"), VarType::fromString("string")}, VarType::fromString("SQLResult"), MysqlDriver::MysqlDriver_Execute);

   /**
    * @class MysqlDriver
     * Escapes the provided value and return the escaped string, securing against SQL Injection.
     * Note this function is also accessible from SQLConnection and is easier to use. We recommend using the escape method within the SQLConnection class
     * function escape(SQLConnction connection, string value) : string
     */
    Function* escape = c->registerFunction("escape", {VarType::fromString("SQLConnection"), VarType::fromString("string")}, VarType::fromString("string"), MysqlDriver::MysqlDriver_Escape);
    

}

std::shared_ptr<Object> MysqlDriver::newInstance(Class* c)
{
    return std::make_shared<MysqlDriver>(c);
}

void MysqlDriver::MysqlDriver_Execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Default return value is NULL
    return_value->type = VALUE_TYPE_OBJECT;
    return_value->ovalue = NULL;


    std::shared_ptr<MysqlConnection> connection = std::dynamic_pointer_cast<MysqlConnection>(values[0].ovalue);
    // If we fail to cast the connection to a MysqlConnection then we must throw a SQLConnectionException as they have passed an invalid connection to us
    if (connection == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException"))), "", interpreter->getStackTraceLog());

    // If this MysqlConnection does not have a real Mysql connection then we cannot proceed.
    if (connection->mysql_connection == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException"))), "This connection was closed or was never opened", interpreter->getStackTraceLog());

    std::string finalized_query = values[2].svalue;
    // Let's execute the finalized query. mysql_query returns 0 for success
    if (mysql_query(connection->mysql_connection, finalized_query.c_str()))
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SQLQueryException"))), "Error with query: " + finalized_query, interpreter->getStackTraceLog());
    } 

    // Let's get the result
    MYSQL_RES* response = mysql_store_result(connection->mysql_connection);
    // If there was no result then we are done here.
    if (response == NULL)
        return;

    // A temporary return value for when we do not care about what is returned
    Value tmpReturnValue;

    // Let's create a SQLResult object which is where we will store our SQLRecord's
    std::shared_ptr<CommonModule_SqlResult> result = 
         std::dynamic_pointer_cast<CommonModule_SqlResult>(Object::create(interpreter->getClassSystem()->getClassByName("SQLResult")));
    Function* result_add_record_func = result->getClass()->getFunctionByNameAndArguments("addRecord", {VarType::fromString("SQLRecord")});
    std::vector<MYSQL_FIELD *> fields;
    MYSQL_FIELD *field;
    MYSQL_ROW row;
    while((field = mysql_fetch_field(response)))
    {
        fields.push_back(field);
    }

    while((row = mysql_fetch_row(response)))
    {
         // Create the record in memory so we have a record to put in the result
        std::shared_ptr<CommonModule_SqlRecord> record = 
            std::dynamic_pointer_cast<CommonModule_SqlRecord>(Object::create(interpreter->getClassSystem()->getClassByName("SQLRecord")));
        Function* record_set_column_func = record->getClass()->getFunctionByNameAndArguments("setColumn", {VarType::fromString("string"), VarType::fromString("string")});

        unsigned long* lengths = mysql_fetch_lengths(response);
        for (int i = 0; i < fields.size(); i++)
        {
            std::string row_field_value = "";
            for (int s = 0; s < lengths[i]; s++)
            {
                row_field_value += row[i][s];
            }
    
            // Invoke the setColumn function in the SQLRecord object
            record_set_column_func->invoke(interpreter, {std::string(fields[i]->name), row_field_value}, &tmpReturnValue, record, caller_scope);
        }

        // We are done with this row so now add it to the result
        result_add_record_func->invoke(interpreter, {Value(record)}, &tmpReturnValue, result, caller_scope);
    }

    // We don't need this mysql_result anymore lets free it
    mysql_free_result(response);

    // We are returning this SQLResult object
    return_value->ovalue = result;
}

void MysqlDriver::MysqlDriver_Connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    MYSQL* mysql_conn = mysql_init(NULL);
    std::string host = values[0].svalue;
    std::string username = values[1].svalue;
    std::string password = values[2].svalue;
    std::string database = values[3].svalue;
    if(!mysql_real_connect(mysql_conn, host.c_str(), username.c_str(), password.c_str(), database.c_str(), 0, NULL, 0))
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException"))), "", interpreter->getStackTraceLog());
    }

    // Now that we have connected to MYSQL lets now create a MysqlConnection and store this MYSQL connection object in there
    Class* mysql_connection_class = interpreter->getClassSystem()->getClassByName("MysqlConnection");
    std::shared_ptr<MysqlConnection> mysql_connection_obj = std::dynamic_pointer_cast<MysqlConnection>(Object::create(interpreter, mysql_connection_class, {object}));
    mysql_connection_obj->mysql_connection = mysql_conn;

    // Return the connection object to the user
    return_value->set(mysql_connection_obj);
}

void MysqlDriver::MysqlDriver_Escape(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Here we should escape the string provided to us
    std::shared_ptr<MysqlDriver> driver = std::dynamic_pointer_cast<MysqlDriver>(object);
    std::shared_ptr<MysqlConnection> connection = std::dynamic_pointer_cast<MysqlConnection>(values[0].ovalue);
    // If we could not cast this connection to a MysqlConnection it means an invalid connection has been passed to this function.
    if (connection == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidEntityException"))), "", interpreter->getStackTraceLog());
    
    std::string value_to_escape = values[1].svalue;
    char escaped[value_to_escape.size()];
    MYSQL* mysql = connection->mysql_connection;
    if (!mysql)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException"))), "This connection was closed or was never opened", interpreter->getStackTraceLog());
    }
    // Now lets escape that string!
    if(mysql_real_escape_string(mysql, escaped, value_to_escape.c_str(), value_to_escape.size()) == -1)
    {
        // Tmp exception should be changed
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("Exception"))), "", interpreter->getStackTraceLog());
    }

    return_value->set(std::string(escaped));
}