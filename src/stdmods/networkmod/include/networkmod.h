#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H
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

    static size_t CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
private:

};
#endif