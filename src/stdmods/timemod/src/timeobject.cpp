#include "timeobject.h"
#include "modulesystem.h"
#include <sstream>
#include <time.h>
TimeObject::TimeObject(Class* c) : Object(c)
{

}

TimeObject::~TimeObject()
{

}

std::shared_ptr<Object> TimeObject::newInstance(Class* c)
{

}

Class* TimeObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("Time");
    c->registerFunction("getTimeStamp", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        Time_getTimeStamp(interpreter, arguments, return_value, object);
    });
}

void TimeObject::Time_getTimeStamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    time_t t = time(0);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = (double) t;
}