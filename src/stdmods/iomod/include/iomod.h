#ifndef IOMODULE_H
#define IOMODULE_H
#include "module.h"
class IOModule : public Module
{
public:
    IOModule();
    virtual ~IOModule();
    virtual void Init();
};
#endif