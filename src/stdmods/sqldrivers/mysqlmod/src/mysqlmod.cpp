#include "mysqlmod.h"
#include "mysqlconnection.h"
#include "mysqldriver.h"

MysqlModule::MysqlModule() : Module("Mysql Module", "MysqlModule", MODULE_TYPE_MARBLE_LIBRARY)
{

}

MysqlModule::~MysqlModule()
{

}

void MysqlModule::Init()
{
    // Let's setup
    MysqlConnection::Init(this->getModuleSystem());
    MysqlDriver::Init(this->getModuleSystem());
}

void MysqlModule::newInterpreter(Interpreter* interpreter)
{

}
