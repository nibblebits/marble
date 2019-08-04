#ifndef SOCKETADDRESSOBJECT_H
#define SOCKETADDRESSOBJECT_H

#include "object.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "modulesystem.h"
#include "interpreter.h"
#include "class.h"



class SocketAddressObject : public Object
{
public:
    SocketAddressObject(Class* c);
    virtual ~SocketAddressObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);


    static void registerClass(ModuleSystem* moduleSystem);


    // Native SocketAddress methods
    static void SocketAddress_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


    struct sockaddr_in cli_addr;
    socklen_t cli_len;
};
#endif