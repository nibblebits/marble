#include "commonmod_sqlstatement.h"
#include "modulesystem.h"
CommonModule_SqlStatement::CommonModule_SqlStatement(Class* c) : Object(c)
{

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
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLStatement");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlStatement>(c));
    c->is_pure = true;

    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlStatement::SQLStatement_Construct);
}


    // Native SQLStatement functions
void CommonModule_SqlStatement::SQLStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{

}