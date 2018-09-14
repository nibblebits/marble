#include "commonmod_preparedstatement.h"
#include "commonmod_sqlconnection.h"
#include "exceptions/systemexception.h"
#include "function.h"
#include "exceptionobject.h"
#include "module.h"
#include "object.h"
#include <iostream>
CommonModule_PreparedStatement::CommonModule_PreparedStatement(Class* c) : CommonModule_SqlStatement(c)
{

}
CommonModule_PreparedStatement::~CommonModule_PreparedStatement()
{

}
std::shared_ptr<Object> CommonModule_PreparedStatement::newInstance(Class* c)
{
    return std::make_shared<CommonModule_PreparedStatement>(c);
}
    
Class* CommonModule_PreparedStatement::registerClass(ModuleSystem* moduleSystem)
{
    ClassSystem* class_system = moduleSystem->getClassSystem();
    Class* c = class_system->registerClass("PreparedStatement", class_system->getClassByName("SQLStatement"));
    c->setDescriptorObject(std::make_shared<CommonModule_PreparedStatement>(c));

    c->registerFunction("__construct", {VarType::fromString("SQLConnection")}, VarType::fromString("void"), PreparedStatement_Construct);
    c->registerFunction("finalizeQuery", {}, VarType::fromString("string"), CommonModule_PreparedStatement::PreparedStatement_finalizeQuery);
    c->registerFunction("addParameter", {VarType::fromString("string")}, VarType::fromString("void"), CommonModule_PreparedStatement::PreparedStatement_addParameter);
}


// Native PreparedStatement functions
void CommonModule_PreparedStatement::PreparedStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
   // Invoke the parent constructor
   object->getClass("PreparedStatement")->invokeObjectParentConstructor(values, object, interpreter);
}

void CommonModule_PreparedStatement::PreparedStatement_finalizeQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_PreparedStatement> statement = std::dynamic_pointer_cast<CommonModule_PreparedStatement>(object);
    // We need to execute the escape function in the connection class to escape our strings in our prepared query
    Function* escape_function = statement->connection->getClass()->getFunctionByNameAndArguments("escape", {VarType::fromString("string")});
    std::string query = statement->query;
    std::string new_query = "";
    int valueIndex = 0;
    for (char c : query)
    {
        if (c != '?')
        {
            new_query += c;
            continue;
        }

        // We have a ? which means its a value to escape
        if (valueIndex >= statement->values.size())
        {
            // We are trying to use a value that has not been set
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("InvalidIndexException"))), "", interpreter->getStackTraceLog());
        }
        Value escape_ret_val;
        escape_function->invoke(interpreter, {statement->values[valueIndex]}, &escape_ret_val, statement->connection, caller_scope);
        valueIndex++;
        new_query += escape_ret_val.svalue;
    }

    // Return our finalized query
    return_value->set(new_query);
   
}

void CommonModule_PreparedStatement::PreparedStatement_addParameter(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_PreparedStatement> statement = std::dynamic_pointer_cast<CommonModule_PreparedStatement>(object);
    statement->values.push_back(values[0].svalue);
}