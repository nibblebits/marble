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
};
#endif