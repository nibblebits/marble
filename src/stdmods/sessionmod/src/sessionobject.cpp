#include "sessionobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <regex>

SessionObject::SessionObject(Class* c) : SessionValuesObject(c)
{

}

SessionObject::~SessionObject()
{

}

void SessionObject::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class Session extends SessionValues
     * 
     * The Session class is responsible for creating, accessing and maintaining sessions
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("Session", moduleSystem->getClassSystem()->getClassByName("SessionValues"));
    c->is_pure = true;
    c->setDescriptorObject(std::make_shared<SessionObject>(c));

    /**
     * @class Session
     * Constructs this Session object
     * 
     * function __construct() : void
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);


    /**
     * @class Session
     * Creates or loads the session with the provided key.
     * pure function create(string session_key) : void
     */
    Function* create_func = c->registerFunction("create", {VarType::fromString("string")}, VarType::fromString("void"), SessionObject::Session_Create);

    /**
     * @class Session
     * Saves the current session.
     * This method is called before destruction of a Session object
     * pure function save() : void
     */
    Function* save_func = c->registerFunction("save", {}, VarType::fromString("void"), Function::Blank);
    create_func->is_pure = true;

}

std::shared_ptr<Object> SessionObject::newInstance(Class* c)
{
    return std::make_shared<SessionObject>(c);
}



void SessionObject::Session_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // We need to ensure the password provided is in a valid format to prevent directory attacks
    std::string sess_password = values[0].svalue;
    std::regex reg("(\\+|-)?[a-zA-Z0-9]+");
    std::cmatch m;
    if (!regex_match(sess_password, reg))
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "Your session password must be alphanumeric characters a-z, A-Z and 0-9");
    }

}