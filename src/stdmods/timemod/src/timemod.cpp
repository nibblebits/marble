#include "timemod.h"
#include "object.h"
#include <sstream>
#include <time.h>
TimeModule::TimeModule() : Module("Time Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

TimeModule::~TimeModule()
{

}

void TimeModule::Init()
{
    log("Time Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("Time");
    c->registerFunction("getTimeStamp", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        Time_getTimeStamp(interpreter, arguments, return_value, object);
    });

    log("Time Module Initialised.", LOG_LEVEL_NOTICE);
}

void TimeModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to IO. 
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Time", "Time", std::make_shared<Object>(this->getModuleSystem()->getClassSystem()->getClassByName("Time")));
}

// Native IO functions/methods
void TimeModule::Time_getTimeStamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    time_t t = time(0);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = (double) t;
}
