#include "mathmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "mathobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

MathModule::MathModule() : Module("mathmod", "Math Module", MODULE_TYPE_MARBLE_LIBRARY, "1.0.0", "ADD_HELP_PATH_HERE")
{

}

MathModule::~MathModule()
{

}

void MathModule::Init()
{
    log("Math Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);

    // Register the Math class
    MathObject::registerClass(this->getModuleSystem());

    log("Math Module Initialised.", LOG_LEVEL_NOTICE);
}

void MathModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to Math. 
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Math", "Math", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Math"), {}));
}
