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
    // Native Math functions/methods
    static void Socket_connect(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif