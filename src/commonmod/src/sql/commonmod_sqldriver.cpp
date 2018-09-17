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

#include "commonmod_sqldriver.h"
#include "commonmod_sqlstatement.h"
#include "commonmod_preparedstatement.h"
#include "modulesystem.h"
#include "function.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <iostream>
CommonModule_SqlDriver::CommonModule_SqlDriver(Class* c) : Object(c)
{

}

CommonModule_SqlDriver::~CommonModule_SqlDriver()
{

}

std::shared_ptr<Object> CommonModule_SqlDriver::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlDriver>(c);
}
    
Class* CommonModule_SqlDriver::registerClass(ModuleSystem* moduleSystem)
{
    // Register a driver not found exception, thrown when a driver does not exist.
    /**
     * pure class DriverNotFoundException extends Exception
     * 
     * This exception is thrown if the SQLDriver could not be resolved
     */
    Class* exception = moduleSystem->getClassSystem()->registerClass("DriverNotFoundException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    /**
     * @class DriverNotFoundException
     * 
     * Constructs this DriverNotFoundException
     */
    exception->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * pure class SQLDriver
     * 
     * The SQLDriver class is to be extended by all SQLDrivers you do not need to bother yourself with this unless
     * you are wanting to create a SQLDriver of your own
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLDriver");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlDriver>(c));
    c->is_pure = true;


    /**
     * @class SQLDriver
     * 
     * Constructs this SQLDriver
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlDriver::SQLDriver_Construct);
    /**
     * @class SQLDriver
     * 
     * Connects to the SQL database based on the credentials provided.
     * This is to be extended by SQL drivers
     *
     * pure function connect(string host, string username, string password, string database) : SQLConnection
     */
    Function* connect_function = c->registerFunction("connect", {VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string")},
                                    VarType::fromString("SQLConnection"), Function::Blank);

    connect_function->is_pure = true;

    /**
     * Registers a SQLDriver into the system
     * 
     * Example usage: <code>SQLDriver.registerDriver(driver_here);</code>
     * function registerDriver(SQLDriver driver) : void
     */
    c->registerFunction("registerDriver", {VarType::fromString("SQLDriver")}, VarType::fromString("void"), CommonModule_SqlDriver::SQLDriver_registerDriver);

    /**
     * 
     * Returns the SQL driver with the given name
     * 
     * Example usage: <code>SQLDriver.getDriver(driver_name);</code>
     * function getDriver(string driver_name) : SQLDriver
     */
    c->registerFunction("getDriver", {VarType::fromString("string")}, VarType::fromString("SQLDriver"), CommonModule_SqlDriver::SQLDriver_getDriver);

    /**
     * Returns true if the driver with the given name is registered to the system
     * 
     * Example usage: <code>SQLDriver.hasDriver(driver_name);</code>
     * function hasDriver(string driver_name) : boolean
     */
    c->registerFunction("hasDriver", {VarType::fromString("string")}, VarType::fromString("boolean"), CommonModule_SqlDriver::SQLDriver_hasDriver);

    /**
     * Executes the given SQL statement by providing a connection, statement and finialized query to this execute function.
     * You should not call this directly instead call <i>execute()</i> within the Statement and PreparedStatement classes
     * 
     * SQLDriver's implement this pure abstract method
     * 
     * pure function execute(SQLConnection connection, SQLStatement statement, string finalized_query) : void
     */
    Function* execute = c->registerFunction("execute", {VarType::fromString("SQLConnection"), VarType::fromString("SQLStatement"), VarType::fromString("string")}, VarType::fromString("SQLResult"), Function::Blank);
    execute->is_pure = true;

    /**
     * Escapes a given string preventing SQL injection
     * 
     * Should be overrided by drivers and sql escape the provided value and return the escaped string
     * function escape(SQLConnction connection, string value) : string
     */
    Function* escape = c->registerFunction("escape", {VarType::fromString("SQLConnection"), VarType::fromString("string")}, VarType::fromString("string"), Function::Blank);
    escape->is_pure = true;
}

void CommonModule_SqlDriver::newInterpreter(Interpreter* interpreter)
{
    // Create a variable named SQLDriver that just points to an object instance of SQLDriver
    interpreter->getCurrentScope()->createVariable("SQLDriver", "SQLDriver", Object::create(interpreter, interpreter->getClassSystem()->getClassByName("SQLDriver"), {}));
}

// Native SQLDriver functions
void CommonModule_SqlDriver::SQLDriver_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Invoke the parent constructor
    object->getClass("SQLDriver")->invokeObjectParentConstructor(values, object, interpreter);
}

void CommonModule_SqlDriver::SQLDriver_registerDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    interpreter->registerSQLDriver(std::dynamic_pointer_cast<CommonModule_SqlDriver>(values[0].ovalue));
}

void CommonModule_SqlDriver::SQLDriver_getDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlDriver> selected_driver = interpreter->getSQLDriver(values[0].svalue);
    if (selected_driver == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DriverNotFoundException"))), "", interpreter->getStackTraceLog());

    // We have a driver, lets now return it to the user
    return_value->set(selected_driver);

}

void CommonModule_SqlDriver::SQLDriver_hasDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlDriver> selected_driver = interpreter->getSQLDriver(values[0].svalue);
    return_value->set(selected_driver != NULL);
}