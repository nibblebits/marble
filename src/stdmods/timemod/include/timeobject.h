#ifndef TIMEOBJECT_H
#define TIMEOBJECT_H

#include "object.h"
class ModuleSystem;
class TimeObject : public Object
{
public:
    TimeObject(Class* c);
    virtual ~TimeObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);
private:
    // Native Time functions/methods
    static void Time_getTimeStamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
};

#endif