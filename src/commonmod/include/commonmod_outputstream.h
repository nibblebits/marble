#ifndef COMMONMODULE_OUTPUTSTREAM
#define COMMONMODULE_OUTPUTSTREAM
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>
class CommonModule_OutputStream : public Object
{
public:
    CommonModule_OutputStream(Class* c);
    virtual ~CommonModule_OutputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    // A char buffer for the output stream
    std::vector<char> buffer;

    // The amount of bytes that must be in the buffer to flush automatically
    int amount_to_auto_flush;

    // Native OutputStream functions
    static void OutputStream_Write(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Get(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Empty(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Size(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Flush(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void OutputStream_Print(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};


#endif