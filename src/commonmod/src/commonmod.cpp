#include "commonmod.h"
#include "object.h"
#include "function.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include <stdio.h>

CommonModule::CommonModule() : Module("commonmod", "Common Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

CommonModule::~CommonModule()
{

}

void CommonModule::Init()
{
    log("Common Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    
    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("OutputStream");
    c->is_pure = true;
    c->setDescriptorObject(std::make_shared<CommonModule_OutputStream>(c));
    Function* f = c->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        std::cout << "CALLED: " << arguments[0].dvalue << std::endl;
    });
    f->is_pure = true;

    f = c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object) {
        Function* write_function = object->getClass()->getFunctionByNameAndArguments("write", {VarType::fromString("number")});
        std::string value = arguments[0].svalue;
        for (int i = 0; i < value.size(); i++)
        {
            Value v;
            v.type = VALUE_TYPE_NUMBER;
            v.set((double) value[i]);
            write_function->invoke(interpreter, {v}, NULL, object);
            std::cout << "FAKE PRINT CALLED" << std::endl;
        }
    });
    log("File Module Initialised.", LOG_LEVEL_NOTICE);
}

void CommonModule::newInterpreter(Interpreter* interpreter)
{
  
}

// Native IO functions/methods
