#ifndef COMMONMODULE_H
#define COMMONMODULE_H
#include "module.h"
#include "object.h"
#include "commonmod_outputstream.h"
#include <stdio.h>
#include <string>

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