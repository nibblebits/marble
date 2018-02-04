#ifndef TIMEMODULE_H
#define TIMEMODULE_H
#include "module.h"
class TimeModule : public Module
{
public:
    TimeModule();
    virtual ~TimeModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:
    // Native Time functions/methods
    void Time_getTimeStamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object);
};
#endif