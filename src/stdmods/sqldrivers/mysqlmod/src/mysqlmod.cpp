#include "mysqlmod.h"
#include "mysqlconnection.h"
#include "mysqldriver.h"

MysqlModule::MysqlModule() : Module("mysqlmod", "Mysql Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "HELP PATH HERE")
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
    // Register the MYSQLDriver in the interpreter instance
    std::shared_ptr<MysqlDriver> mysql_driver 
                        = std::dynamic_pointer_cast<MysqlDriver>(Object::create(interpreter->getClassSystem()->getClassByName("MysqlDriver")));
    interpreter->registerSQLDriver(mysql_driver);
}


void MysqlModule::unload()
{
    // Shutdown the mysql library
    mysql_library_end();
}