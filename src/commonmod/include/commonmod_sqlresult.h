#ifndef SQLRESULT_H
#define SQLRESULT_H
#include "object.h"
#include <deque>

/**
 * Class for SQL Results containing all records for a particular query.
 */

class ModuleSystem;
class CommonModule_SqlRecord;
class CommonModule_SqlResult : public Object
{
public:
    CommonModule_SqlResult(Class* c);
    virtual ~CommonModule_SqlResult();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    // Native SQLResult functions
    static void SQLResult_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLResult_addRecord(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLResult_getNextRecord(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    std::deque<std::shared_ptr<CommonModule_SqlRecord>> records; 
    
};

#endif
