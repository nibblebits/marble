#ifndef SHELLEXECUTIONOBJECT_H
#define SHELLEXECUTIONOBJECT_H

#include "object.h"
class ModuleSystem;
class ShellExecutionObject : public Object
{
public:
    ShellExecutionObject(Class* c);
    virtual ~ShellExecutionObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);

    // The response code from this ShellExecution
    int response_code;

    // The response from this ShellExecution
    std::string response;

    // Native ShellExecution methods
    static void getResponseCode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void getResponse(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif