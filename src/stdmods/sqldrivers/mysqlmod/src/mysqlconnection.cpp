#include "mysqlconnection.h"
#include "modulesystem.h"
MysqlConnection::MysqlConnection(Class* c) : CommonModule_SqlConnection(c)
{
    this->mysql_connection = NULL;
}

MysqlConnection::~MysqlConnection()
{

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
}

void MysqlConnection::MysqlConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Invoke the parent constructor of MysqlConnection.
    object->getClass("MysqlConnection")->invokeObjectParentConstructor(values, object, interpreter);
}
