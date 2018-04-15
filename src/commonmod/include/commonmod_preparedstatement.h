#ifndef COMMONMOD_PREPAREDSTATEMENT
#define COMMONMOD_PREPAREDSTATEMENT

#include "commonmod_sqlstatement.h"
#include <deque>
class ModuleSystem;
/**
 * Class for Prepared SQL statements
 */
class CommonModule_PreparedStatement : public CommonModule_SqlStatement
{
public:
    CommonModule_PreparedStatement(Class* c);
    virtual ~CommonModule_PreparedStatement();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);


    // Native PreparedStatement functions
    static void PreparedStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void PreparedStatement_finalizeQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void PreparedStatement_addParameter(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
   
    // The provided values of this prepared statement, these are the values to be escaped
    std::deque<std::string> values;
};

#endif