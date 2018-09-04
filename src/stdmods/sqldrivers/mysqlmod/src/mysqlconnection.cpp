#include "mysqlconnection.h"
#include "modulesystem.h"
MysqlConnection::MysqlConnection(Class* c) : CommonModule_SqlConnection(c)
{
    this->mysql_connection = NULL;
}

MysqlConnection::~MysqlConnection()
{
    // Did the programmer close the connection? If not then we need to do it for them
    if (this->mysql_connection != NULL)
        mysql_close(this->mysql_connection);
}

std::shared_ptr<Object> MysqlConnection::newInstance(Class* c)
{
    return std::make_shared<MysqlConnection>(c);
}

void MysqlConnection::Init(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("MysqlConnection", moduleSystem->getClassSystem()->getClassByName("SQLConnection"));
    c->setDescriptorObject(std::make_shared<MysqlConnection>(c));
    c->registerFunction("__construct", {VarType::fromString("SQLDriver")}, VarType::fromString("void"), MysqlConnection::MysqlConnection_Construct);

    /**
     * @class MysqlConnection
     * Closes this MYSQL connection
     * function close() : void
     */
    Function* close_function = c->registerFunction("close", {}, VarType::fromString("void"), MysqlConnection::MysqlConnection_Close);
}

void MysqlConnection::MysqlConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Invoke the parent constructor of MysqlConnection.
    object->getClass("MysqlConnection")->invokeObjectParentConstructor(values, object, interpreter);
}

void MysqlConnection::MysqlConnection_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Let's close the MYSQL connection
    std::shared_ptr<MysqlConnection> mysql_conn_obj = std::dynamic_pointer_cast<MysqlConnection>(object);
    mysql_close(mysql_conn_obj->mysql_connection);
    mysql_conn_obj->mysql_connection = NULL;
}




