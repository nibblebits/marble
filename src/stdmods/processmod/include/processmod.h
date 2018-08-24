#ifndef PROCESSMODULE_H
#define PROCESSMODULE_H
#include "module.h"
class Scope;
/**
 * This is the Process module it is responsible for all things in marble relating to processes such as shell execution
 */
class ProcessModule : public Module
{
public:
    ProcessModule();
    virtual ~ProcessModule();
    virtual void Init();
    void newInterpreter(Interpreter* interpreter);

private:

};
#endif