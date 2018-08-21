#ifndef SOCKETMODULE_H
#define SOCKETMODULE_H
#include "module.h"
class Scope;
/**
 * This is the Socket module and is in charge of socket communication in marble
 */
class SocketModule : public Module
{
public:
    SocketModule();
    virtual ~SocketModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
private:

};
#endif