#ifndef JSONMODULE_H
#define JSONMODULE_H
#include "module.h"
class Scope;
/**
 * This is the CryptoModule it is responsible for all things in marble relating to encryption and hashing
 */
class CryptoModule : public Module
{
public:
    CryptoModule();
    virtual ~CryptoModule();
    virtual void Init();
    void newInterpreter(Interpreter* interpreter);

private:

};
#endif