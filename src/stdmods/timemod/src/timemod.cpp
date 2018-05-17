#include "timemod.h"
#include "object.h"
#include "timeobject.h"
#include "dateobject.h"
#include <sstream>
#include <time.h>
TimeModule::TimeModule() : Module("timemod", "Time Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

TimeModule::~TimeModule()
{

}

void TimeModule::Init()
{
    log("Time Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    TimeObject::registerClass(getModuleSystem());
    DateObject::registerClass(getModuleSystem());

    log("Time Module Initialised.", LOG_LEVEL_NOTICE);
}

void TimeModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to IO. 
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Time", "Time", Object::create(this->getModuleSystem()->getClassSystem()->getClassByName("Time")));
}
