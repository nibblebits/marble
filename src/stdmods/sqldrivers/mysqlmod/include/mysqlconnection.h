#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H
#include "../../../../commonmod/include/commonmod_sqlconnection.h"
#include <my_global.h>
#include <mysql.h>
class MysqlConnection : public CommonModule_SqlConnection
{
public:
    MysqlConnection(Class* c);
    virtual ~MysqlConnection();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void Init(ModuleSystem* moduleSystem);

    // Native MysqlConnection functions
    static void MysqlConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MysqlConnection_Close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    MYSQL* mysql_connection;
};

#endif