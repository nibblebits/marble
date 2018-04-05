#ifndef COMMONMOD_SQLSTATEMENT
#define COMMONMOD_SQLSTATEMENT

#include "object.h"
class ModuleSystem;
class CommonModule_SqlDriver;

/**
 * Class for SQL statements
 */
class CommonModule_SqlStatement : public Object
{
public:
    CommonModule_SqlStatement(Class* c);
    virtual ~CommonModule_SqlStatement();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);


    // Native SQLStatement functions
    static void SQLStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


private:
    // The driver for this SQLStatement
    std::shared_ptr<CommonModule_SqlDriver> driver;
};

#endif