#include "commonmod_sqlstatement.h"
#include "commonmod_sqldriver.h"
#include "modulesystem.h"
CommonModule_SqlStatement::CommonModule_SqlStatement(Class* c) : Object(c)
{
    this->driver = NULL;
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

    // function __construct(SQLDriver driver) : void
    c->registerFunction("__construct", {VarType::fromString("SQLDriver")}, VarType::fromString("void"), CommonModule_SqlStatement::SQLStatement_Construct);
}


// Native SQLStatement functions
void CommonModule_SqlStatement::SQLStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Set the statement's driver to the one provided
    std::shared_ptr<CommonModule_SqlStatement> statement = std::dynamic_pointer_cast<CommonModule_SqlStatement>(object);
    statement->driver = std::dynamic_pointer_cast<CommonModule_SqlDriver>(values[0].ovalue);
}