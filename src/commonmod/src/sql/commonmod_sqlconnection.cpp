#include "commonmod_sqlconnection.h"
#include "commonmod_sqldriver.h"
#include "commonmod_sqlstatement.h"
#include "commonmod_preparedstatement.h"
#include "module.h"
#include "function.h"
#include <iostream>


CommonModule_SqlConnection::CommonModule_SqlConnection(Class* c) : Object(c)
{

}

CommonModule_SqlConnection::~CommonModule_SqlConnection()
{

}

std::shared_ptr<Object> CommonModule_SqlConnection::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlConnection>(c);
}
    
Class* CommonModule_SqlConnection::registerClass(ModuleSystem* moduleSystem)
{
    moduleSystem->getClassSystem()->registerClass("SQLConnectionException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    /**
     * class SQLConnection
     * 
     * The SQLConnection class is the base class of all SQLConnection's and contains base class information such as pure methods
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLConnection");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlConnection>(c));
    c->is_pure = true;
    c->registerFunction("__construct", {VarType::fromString("SQLDriver")}, VarType::fromString("void"), CommonModule_SqlConnection::SQLConnection_Construct);
    c->registerFunction("getStatement", {}, VarType::fromString("SQLStatement"), CommonModule_SqlConnection::SQLConnection_getStatement);
    c->registerFunction("getPreparedStatement", {}, VarType::fromString("PreparedStatement"), CommonModule_SqlConnection::SQLConnection_getPreparedStatement);

    /**
     * @SQLConnection
     * Escapes the given value to protect against sql injection and returns the escaped value
     * 
     * function escape(string value) : string
     */
    c->registerFunction("escape", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_SqlConnection::SQLConnection_Escape);

    /**
     * @SQLConnection
     * Returns the last insert id to the database for this SQLConnection.
     * This should be implemented by SQL Driver connections that handle insert ids
     * 
     * function getInsertId() : number
     */
    c->registerFunction("getInsertId", {}, VarType::fromString("number"), Function::Blank);


    /**
     * @class SQLConnection
     * 
     * Closes this connection to the database
     * pure function close() : void
     */
    Function* close_function = c->registerFunction("close", {}, VarType::fromString("void"), Function::Blank);
    close_function->is_pure = true;
}


// Native SQLConnection functions
void CommonModule_SqlConnection::SQLConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Set the connections's driver to the one provided
    std::shared_ptr<CommonModule_SqlConnection> connection = std::dynamic_pointer_cast<CommonModule_SqlConnection>(object);
    connection->driver = std::dynamic_pointer_cast<CommonModule_SqlDriver>(values[0].ovalue);
}


void CommonModule_SqlConnection::SQLConnection_getStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlConnection> connection = std::dynamic_pointer_cast<CommonModule_SqlConnection>(object);
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(Object::create(interpreter, "SQLStatement", {Value(connection)}));
    return_value->set(statement);
}

void CommonModule_SqlConnection::SQLConnection_getPreparedStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlConnection> connection = std::dynamic_pointer_cast<CommonModule_SqlConnection>(object);
    std::shared_ptr<CommonModule_PreparedStatement> statement = std::dynamic_pointer_cast<CommonModule_PreparedStatement>(Object::create(interpreter, "PreparedStatement", {Value(connection)}));
    return_value->set(statement);
}

void CommonModule_SqlConnection::SQLConnection_Escape(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlConnection> connection = std::dynamic_pointer_cast<CommonModule_SqlConnection>(object);
    Function* driver_escape_func = connection->driver->getClass()->getFunctionByNameAndArguments("escape", {VarType::fromString("SQLConnection"), VarType::fromString("string")});
    std::string value_to_escape = values[0].svalue;
    // Let's now invoke the driver escape function and pass ourself and the value to escape, we will also pass our own return value so it returns the escaped query to the programmer
    driver_escape_func->invoke(interpreter, {Value(connection), Value(value_to_escape)}, return_value, connection->driver, caller_scope);

}