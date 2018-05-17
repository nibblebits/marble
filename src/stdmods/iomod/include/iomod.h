#ifndef IOMODULE_H
#define IOMODULE_H
#include "module.h"
class Scope;
class IOModule : public Module
{
public:
    IOModule();
    virtual ~IOModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:
    // Native IO functions/methods
    void setDefaultIO(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    void IO_print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    void IO_input(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};
#endif