#include "mathmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

MathModule::MathModule() : Module("mathmod", "Math Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

MathModule::~MathModule()
{

}

void MathModule::Init()
{
    log("Math Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    

    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("Math");
    /**
     * Returns the sine of an angle of x radians.
     * 
     * function sin(number value) : number
     */
    c->registerFunction("sin", {VarType::fromString("number")}, VarType::fromString("number"), MathModule::Math_sin);

    log("Math Module Initialised.", LOG_LEVEL_NOTICE);
}

void MathModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to Math. 
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("Math", "Math", Object::create(interpreter, this->getModuleSystem()->getClassSystem()->getClassByName("Math"), {}));
}


void MathModule::Math_sin(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->set((double) sin(values[0].dvalue));
}