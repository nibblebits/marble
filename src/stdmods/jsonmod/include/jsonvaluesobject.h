#ifndef JSONVALUESOBJECT_H
#define JSONVALUESOBJECT_H

#include "object.h"
#include "value.h"
#include <map>
#include <string>

class ModuleSystem;
class Value;
class JsonValuesObject : public Object
{
public:
    JsonValuesObject(Class* c);
    virtual ~JsonValuesObject();

    static void registerClass(ModuleSystem* moduleSystem);
    virtual std::shared_ptr<Object> newInstance(Class* c);
    void ensureIndexExists(Interpreter* interpreter, std::string index_name);

    // The value for this session value object
    std::map<std::string, Value> values;

    // Native JsonValues functions/methods
    static void JsonValues_Has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_SetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_SetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_SetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    static void JsonValues_GetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_GetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_GetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void JsonValues_GetArray(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope);

};

#endif