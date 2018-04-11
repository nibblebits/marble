#ifndef SQLRECORD_H
#define SQLRECORD_H
#include "object.h"

/**
 * Class for a single SQL Record and it represents a record that was apart of a SQLResult 
 */

class ModuleSystem;
class CommonModule_SqlRecord : public Object
{
public:
    CommonModule_SqlRecord(Class* c);
    virtual ~CommonModule_SqlRecord();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    
    // Native SQLRecord functions
    static void SQLRecord_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLRecord_setColumn(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLRecord_getValue(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
private:
    // A map where the key is the column name and the value is the value of the column for this record instance.
    std::map<std::string, std::string> column_value;
};

#endif
