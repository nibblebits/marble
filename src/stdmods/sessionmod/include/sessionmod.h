#ifndef SESSIONMODULE_H
#define SESSIONMODULE_H
#include "module.h"
class Scope;
class SessionModule : public Module
{
public:
    SessionModule();
    virtual ~SessionModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:

};
#endif