#include "iomod.h"
#include "object.h"
#include <sstream>
#include <iostream>
IOModule::IOModule() : Module("IO Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

IOModule::~IOModule()
{

}

void IOModule::Init()
{
    log("IO Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering print function", LOG_LEVEL_NOTICE);
    
    this->getModuleSystem()->getFunctionSystem()->registerFunction("setDefaultIO", {VarType::fromString("IO")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        setDefaultIO(interpreter, arguments, return_value, object);
    });

    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("IO");
    c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        IO_print(interpreter, arguments, return_value, object);
    });
    log("IO Module Initialised.", LOG_LEVEL_NOTICE);
}

void IOModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to IO. 
    Scope* root_scope = interpreter->getRootScope();
    Variable* variable = root_scope->createVariable();
    variable->value.type = VALUE_TYPE_OBJECT;
    variable->value.ovalue = std::make_shared<Object>(this->getModuleSystem()->getClassSystem()->getClassByName("IO"));
    variable->value.holder = variable;
    variable->name = "Io";
    variable->access = MODIFIER_ACCESS_PUBLIC;
    variable->type = VARIABLE_TYPE_OBJECT;
    variable->type_name = "IO";
}

// Native IO functions/methods
void IOModule::setDefaultIO(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    
}

void IOModule::IO_print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    std::stringstream ss;
    for (Value v : values)
    {
        if (v.type == VALUE_TYPE_NUMBER)
        {
            ss << v.dvalue;
        }
        else if(v.type == VALUE_TYPE_STRING)
        {
            ss << v.svalue;
        }
        else
        {
            ss << "Invalid value type: " << v.type;
         } 
    }

    interpreter->output(ss.str().c_str());
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = 1;
}