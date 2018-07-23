#include "sessionobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"

SessionObject::SessionObject(Class* c) : SessionValuesObject(c)
{

}

SessionObject::~SessionObject()
{

}

void SessionObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("Session", moduleSystem->getClassSystem()->getClassByName("SessionValues"));
    c->is_pure = true;
    c->setDescriptorObject(std::make_shared<SessionObject>(c));

    /**
     * 
     * Creates or loads the session with the provided key.
     * This Session must be bound to a driver for this method to function
     * pure function create(string session_key) : void
     */
    Function* create_func = c->registerFunction("create", {VarType::fromString("string")}, VarType::fromString("void"), Function::Blank);
    create_func->is_pure = true;



}

std::shared_ptr<Object> SessionObject::newInstance(Class* c)
{
    return std::make_shared<SessionObject>(c);
}



