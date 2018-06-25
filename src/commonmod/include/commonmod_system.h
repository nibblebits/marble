#ifndef COMMONMODULE_SYSTEM
#define COMMONMODULE_SYSTEM
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
class Interpreter;
class CommonModule_System : public Object
{
public:
    CommonModule_System(Class* c);
    virtual ~CommonModule_System();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);
    static void newInterpreter(Interpreter* interpreter);
    static void System_SetTimeout(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};


#endif