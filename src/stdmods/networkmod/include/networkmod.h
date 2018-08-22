#ifndef SOCKETMODULE_H
#define SOCKETMODULE_H
#include "module.h"
class Scope;
/**
 * This is the Network module and is in charge of socket communication and other network operations in marble
 */
class NetworkModule : public Module
{
public:
    NetworkModule();
    virtual ~NetworkModule();
    virtual void Init();
    void newInterpreter(Interpreter* interpreter);
private:

};
#endif