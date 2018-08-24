#ifndef SHELLOBJECT_H
#define SHELLOBJECT_H

#include "object.h"
class ModuleSystem;
class ShellObject : public Object
{
public:
    ShellObject(Class* c);
    virtual ~ShellObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);

    // Native Shell methods
    static void Shell_execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif