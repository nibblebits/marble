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
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    this->getModuleSystem()->getFunctionSystem()->registerFunction("setDefaultIO", {VarType::fromString("IO")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        setDefaultIO(interpreter, arguments, return_value, object);
    });

    this->getModuleSystem()->getFunctionSystem()->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        IO_print(interpreter, arguments, return_value, object);
    });

    this->getModuleSystem()->getFunctionSystem()->registerFunction("input", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        IO_input(interpreter, arguments, return_value, object);
    });

    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("IO");
    c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        IO_print(interpreter, arguments, return_value, object);
    });

    c->registerFunction("input", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        IO_input(interpreter, arguments, return_value, object);
    });
    
    log("IO Module Initialised.", LOG_LEVEL_NOTICE);
}

void IOModule::newInterpreter(Interpreter* interpreter)
{
    // Ok we have a new Interpreter that may use this module so we must create a global variable pointing to IO. 
    Scope* root_scope = interpreter->getRootScope();
    root_scope->createVariable("IO", "IO", std::make_shared<Object>(this->getModuleSystem()->getClassSystem()->getClassByName("IO")));
    log("VARIABLE IO CREATED IN ROOT SCOPE", LOG_LEVEL_NOTICE);
}

// Native IO functions/methods
void IOModule::setDefaultIO(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    Scope* root_scope = interpreter->getCurrentScope();
    Variable* variable = root_scope->getVariable("IO");
    variable->setValue(values[0].ovalue);
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

void IOModule::IO_input(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    std::string val = interpreter->input();
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = val;
}