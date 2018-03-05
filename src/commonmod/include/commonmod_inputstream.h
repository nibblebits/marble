#ifndef COMMONMODULE_INPUTSTREAM
#define COMMONMODULE_INPUTSTREAM
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
#include <deque>
class CommonModule_InputStream : public Object
{
public:
    CommonModule_InputStream(Class* c);
    virtual ~CommonModule_InputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    // A char buffer for the input stream
    std::deque<char> buffer;

    // The amount to load into the buffer when calling the fill function
    int amount_to_fill;

    // Native InputStream functions
    static void InputStream_Put(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object);
    static void InputStream_Read(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object);
    static void InputStream_Fill(Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object);
};


#endif