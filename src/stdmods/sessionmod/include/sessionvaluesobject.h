#ifndef SESSIONVALUESOBJECT_H
#define SESSIONVALUESOBJECT_H

#include "object.h"
#include "value.h"
#include <map>
#include <string>

class ModuleSystem;
class Value;
class SessionValuesObject : public Object
{
public:
    SessionValuesObject(Class* c);
    virtual ~SessionValuesObject();

    static void registerClass(ModuleSystem* moduleSystem);
    virtual std::shared_ptr<Object> newInstance(Class* c);
    void ensureIndexExists(Interpreter* interpreter, std::string index_name);


    // The value for this session value object
    std::map<std::string, Value> values;

    // Native SessionValues functions/methods
    static void SessionValues_GetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SessionValues_GetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SessionValues_GetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};

#endif