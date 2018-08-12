#ifndef COMMONMODULE_STRINGUTILS
#define COMMONMODULE_STRINGUTILS
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
class CommonModule_StringUtils : public Object
{
public:
    CommonModule_StringUtils(Class* c);
    virtual ~CommonModule_StringUtils();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    static void StringUtils_getASCIIString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void StringUtils_substr(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void StringUtils_strlen(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void StringUtils_strpos(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void StringUtils_split(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void StringUtils_str_replace(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void StringUtils_preg_match_all(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};


#endif