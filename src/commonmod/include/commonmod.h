#ifndef COMMONMODULE_H
#define COMMONMODULE_H
#include "module.h"
#include "object.h"
#include <stdio.h>
#include <string>

class CommonModule_OutputStream : public Object
{
public:
    CommonModule_OutputStream(Class* c);
    virtual ~CommonModule_OutputStream();
    virtual std::shared_ptr<Object> newInstance(Class* c);
};

class CommonModule : public Module
{
public:
    CommonModule();
    virtual ~CommonModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:
    // Native functions/methods

};
#endif