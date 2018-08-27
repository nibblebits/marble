#ifndef JSONMODULE_H
#define JSONMODULE_H
#include "module.h"
class Scope;
/**
 * This is the Process module it is responsible for all things in marble relating to processes such as shell execution
 */
class JsonModule : public Module
{
public:
    JsonModule();
    virtual ~JsonModule();
    virtual void Init();
    void newInterpreter(Interpreter* interpreter);

private:

};
#endif