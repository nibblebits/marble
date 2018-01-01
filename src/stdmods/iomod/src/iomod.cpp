#include "iomod.h"
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
    this->getModuleSystem()->getFunctionSystem()->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        print(interpreter, arguments, return_value, object);
    });
    log("IO Module Initialised.", LOG_LEVEL_NOTICE);
}

// Native IO functions/methods
void IOModule::print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
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