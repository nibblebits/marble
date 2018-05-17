#ifndef DATEOBJECT_H
#define DATEOBJECT_H

#include "object.h"
#include <ctime>
#include <string>
class ModuleSystem;
class DateObject : public Object
{
public:
    DateObject(Class* c);
    virtual ~DateObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    static Class* registerClass(ModuleSystem* moduleSystem);
private:
    // The unix timestamp for this date
    time_t time;
    std::string format;
    // Native Time functions/methods
    static void Date_ConstructWithTimestamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
    static void Date_setFormat(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);  
    static void Date_getFormattedString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
};

#endif