#include "socketobject.h"
#include "function.h"
#include "interpreter.h"
SocketObject::SocketObject(Class *c) : Object(c)
{
    this->sockfd = -1;
    this->family = -1;
    this->type = -1;
}

SocketObject::~SocketObject()
{
}

void SocketObject::registerClass(ModuleSystem *moduleSystem)
{

    
    /// Register a socket exception class
    /**
     * pure class SocketException extends Exception
     * 
     * This exception is thrown if the there is a problem relating to sockets
     */
    Class *exception = moduleSystem->getClassSystem()->registerClass("SocketException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    /**
     * @class SocketException
     * 
     * Constructs this SocketException
     */
    exception->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * pure class Socket extends Object
     * 
     * Base socket class all sockets extend this class
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("Socket");
    c->is_pure = true;
    c->setDescriptorObject(std::make_shared<SocketObject>(c));

    /**
     * @class Socket
     * 
     * Constructs this pure Socket class
     * 
     * function __construct() : void
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Socket__construct);

}

void SocketObject::newInterpreter(Interpreter *interpreter)
{
    Scope *current_scope = interpreter->getCurrentScope();
    current_scope->createVariable("SOCK_STREAM", 1);
    current_scope->createVariable("SOCK_DGRAM", 2);
    current_scope->createVariable("SOCK_RAW", 3);
    current_scope->createVariable("SOCK_RDM", 4);
    current_scope->createVariable("SOCK_SEQPACKET", 5);

}

std::shared_ptr<Object> SocketObject::newInstance(Class *c)
{
    return std::make_shared<SocketObject>(c);
}


void SocketObject::Socket__construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
}