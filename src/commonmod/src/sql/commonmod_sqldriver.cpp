#include "commonmod_sqldriver.h"
#include "commonmod_sqlstatement.h"
#include "commonmod_preparedstatement.h"
#include "modulesystem.h"
#include "function.h"
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
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLDriver");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlDriver>(c));
    c->is_pure = true;

    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlDriver::SQLDriver_Construct);
    c->registerFunction("getStatement", {}, VarType::fromString("SQLStatement"), SQLDriver_getStatement);
    c->registerFunction("getPreparedStatement", {}, VarType::fromString("PreparedStatement"), SQLDriver_getPreparedStatement);
    
    Function* execute = c->registerFunction("execute", {VarType::fromString("SQLStatement")}, VarType::fromString("void"), Function::Blank);
    execute->is_pure = true;
}


// Native SQLDriver functions
void CommonModule_SqlDriver::SQLDriver_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Invoke the parent constructor
    object->getClass("SQLDriver")->invokeObjectParentConstructor(values, object, interpreter);
}

void CommonModule_SqlDriver::SQLDriver_getStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // The SQLDriver is us the object that was passed
    std::shared_ptr<Object> sql_driver = object;
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(Object::create(interpreter, "SQLStatement", {sql_driver}));
    return_value->set(statement);
}

void CommonModule_SqlDriver::SQLDriver_getPreparedStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // The SQLDriver is us the object that was passed
    std::shared_ptr<Object> sql_driver = object;
    std::shared_ptr<CommonModule_PreparedStatement> statement = std::dynamic_pointer_cast<CommonModule_PreparedStatement>(Object::create(interpreter, "PreparedStatement", {sql_driver}));
    return_value->set(statement);
}