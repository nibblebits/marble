#include "mysqldriver.h"
#include "modulesystem.h"
#include "function.h"
#include "interpreter.h"
#include "mysqlconnection.h"
#include "exceptions/systemexception.h"
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
    Class* c = moduleSystem->getClassSystem()->registerClass("MysqlDriver", moduleSystem->getClassSystem()->getClassByName("SQLDriver"));
    c->setDescriptorObject(std::make_shared<MysqlDriver>(c));
    // function __construct() : void
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    // function connect(string host, string username, string password, string database) : SQLConnection
    Function* connect_function = c->registerFunction("connect", {VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string")},
                                    VarType::fromString("SQLConnection"), MysqlDriver::MysqlDriver_Connect);

    Function* execute = c->registerFunction("execute", {VarType::fromString("SQLConnection"), VarType::fromString("SQLStatement"), VarType::fromString("string")}, VarType::fromString("void"), MysqlDriver::MysqlDriver_Execute);

}

std::shared_ptr<Object> MysqlDriver::newInstance(Class* c)
{
    return std::make_shared<MysqlDriver>(c);
}

void MysqlDriver::MysqlDriver_Execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MysqlConnection> connection = std::dynamic_pointer_cast<MysqlConnection>(values[0].ovalue);
    // If we fail to cast the connection to a MysqlConnection then we must throw a SQLConnectionException as they have passed an invalid connection to us
    if (connection == NULL)
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException")));

    // If this MysqlConnection does not have a real Mysql connection then we cannot proceed.
    if (connection->mysql_connection == NULL)
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException")));

    std::string finalized_query = values[2].svalue;
    // Let's execute the finalized query. mysql_query returns 0 for success
    if (mysql_query(connection->mysql_connection, finalized_query.c_str()))
    {
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("SQLQueryException")));
    } 


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
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("SQLConnectionException")));
    }

    // Now that we have connected to MYSQL lets now create a MysqlConnection and store this MYSQL connection object in there
    Class* mysql_connection_class = interpreter->getClassSystem()->getClassByName("MysqlConnection");
    std::shared_ptr<MysqlConnection> mysql_connection_obj = std::dynamic_pointer_cast<MysqlConnection>(Object::create(interpreter, mysql_connection_class, {object}));
    mysql_connection_obj->mysql_connection = mysql_conn;

    // Return the connection object to the user
    return_value->set(mysql_connection_obj);
}