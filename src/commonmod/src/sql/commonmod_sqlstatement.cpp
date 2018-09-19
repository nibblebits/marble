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

#include "commonmod_sqlstatement.h"
#include "commonmod_sqldriver.h"
#include "commonmod_sqlconnection.h"
#include "function.h"
#include "modulesystem.h"
#include "interpreter.h"
CommonModule_SqlStatement::CommonModule_SqlStatement(Class* c) : Object(c)
{
    this->connection = NULL;
    this->query = "";
}

CommonModule_SqlStatement::~CommonModule_SqlStatement()
{

}

std::shared_ptr<Object> CommonModule_SqlStatement::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlStatement>(c);
}
    
Class* CommonModule_SqlStatement::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class SQLQueryException extends Exception
     * 
     * An instance of this class is thrown if their is an issue with your sql query
     */
    moduleSystem->getClassSystem()->registerClass("SQLQueryException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    /**
     * pure class SQLStatement
     * 
     * The base class for all SQLStatements
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLStatement");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlStatement>(c));
    c->is_pure = true;

    // function __construct(SQLConnection connection) : void
    /**
     * @class SQLStatement
     * 
     * Constructs this sql statement
     * function __construct(SQLConnection connection) : void
     */
    c->registerFunction("__construct", {VarType::fromString("SQLConnection")}, VarType::fromString("void"), CommonModule_SqlStatement::SQLStatement_Construct);
    // function setQuery(string query) : void
    /**
     * @class SQLStatement
     * 
     * Sets the query for this SQLStatement
     * function setQuery(string query) : void
     */
    c->registerFunction("setQuery", {VarType::fromString("string")}, VarType::fromString("void"), CommonModule_SqlStatement::SQLStatement_setQuery);
    /* @class SQLStatement
     * Finalizes the query that was provided with setQuery and returns the finalized query as a string.
     * function finalizeQuery() : string
     */
    c->registerFunction("finalizeQuery", {}, VarType::fromString("string"), CommonModule_SqlStatement::SQLStatement_finalizeQuery);
    /**
     * @class SQLStatement
     * 
     * Executes this given sql query
     *
     * function execute() : void
     */
    c->registerFunction("execute", {}, VarType::fromString("SQLResult"), CommonModule_SqlStatement::SQLStatement_Execute);
}


// Native SQLStatement functions
void CommonModule_SqlStatement::SQLStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Set the statement's driver to the one provided
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(object);
    statement->connection = std::dynamic_pointer_cast<CommonModule_SqlConnection>(values[0].ovalue);
}

 void CommonModule_SqlStatement::SQLStatement_Execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
 {
    // We will call execute on the driver class and pass our statement
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(object);
    // First we need to finalize the query for this statement as we also need to provide a finalized query
    Function* finalizeQueryFunction = statement->getClass()->getFunctionByName("finalizeQuery");
    Value finalizeQueryFunctionResult;
    finalizeQueryFunction->invoke(interpreter, {}, &finalizeQueryFunctionResult, statement, caller_scope);
    std::string finalized_query = finalizeQueryFunctionResult.svalue;
    Function* driver_execute_func = statement->connection->driver->getClass()->getFunctionByNameAndArguments("execute", {VarType::fromString("SQLConnection"), VarType::fromString("SQLStatement"), VarType::fromString("string")}, NULL);
    // Invoke the driver execute function, it will return SQLResult which will then set our own return value.
    driver_execute_func->invoke(interpreter, {Value(statement->connection), Value(statement), Value(finalized_query)}, return_value, statement->connection->driver, caller_scope);
 }

 void CommonModule_SqlStatement::SQLStatement_setQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
 {
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(object);
    statement->query = values[0].svalue;
 }

  void CommonModule_SqlStatement::SQLStatement_finalizeQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
 {
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(object);
    // The SQLStatement does not really need to finalize the query as it does nothing special so just return the same query
    return_value->set(statement->query);
 }
