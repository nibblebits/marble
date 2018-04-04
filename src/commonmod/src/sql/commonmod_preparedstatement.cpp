#include "commonmod_preparedstatement.h"
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
    c->setDescriptorObject(Object::create(c));

    c->registerFunction("__construct", {}, VarType::fromString("void"), PreparedStatement_Construct);
}


// Native PreparedStatement functions
void CommonModule_PreparedStatement::PreparedStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
   
}