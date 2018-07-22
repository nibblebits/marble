#include "sessionobject.h"
SessionObject::SessionObject(Class* c) : Object(c)
{

}

SessionObject::~SessionObject()
{

}

void SessionObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("Session");
    c->setDescriptorObject(std::make_shared<SessionObject>(c));

    /**
     * 
     * Creates or loads the session with the provided key.
     * This Session must be bound to a driver for this method to function
     * function create(string session_key) : void
     */
    Function* create_func = c->registerFunction("create", {VarType::fromString("string")}, VarType::fromString(""));
}

std::shared_ptr<Object> SessionObject::newInstance(Class* c)
{
    return std::make_shared<SessionObject>(c);
}


void SessionObject::Session_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    
}
