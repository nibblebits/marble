#ifndef COMMONMOD_PREPAREDSTATEMENT
#define COMMONMOD_PREPAREDSTATEMENT

#include "commonmod_sqlstatement.h"
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

};

#endif