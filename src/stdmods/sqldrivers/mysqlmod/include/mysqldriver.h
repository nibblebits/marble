#ifndef MYSQLDRIVER_H
#define MYSQLDRIVER_H

#include "../../../../commonmod/include/commonmod_sqldriver.h"
class ModuleSystem;
class Interpreter;
class MysqlDriver : public CommonModule_SqlDriver
{
    public:
        MysqlDriver(Class* c);
        virtual ~MysqlDriver();
        virtual std::shared_ptr<Object> newInstance(Class* c);

        static void Init(ModuleSystem* moduleSystem);

        // Native MysqlDriver functions
        static void MysqlDriver_Execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        static void MysqlDriver_Connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        static void MysqlDriver_Escape(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};

#endif