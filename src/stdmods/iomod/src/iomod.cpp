#include "iomod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "permissionsobject.h"
#include "iopermission.h"
#include "exceptionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>
IOModule::IOModule() : Module("iomod", "IO Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

IOModule::~IOModule()
{

}

void IOModule::Init()
{
    log("IO Module Initialising...", LOG_LEVEL_NOTICE);
    log("--- Registering functions and classes", LOG_LEVEL_NOTICE);
    

    this->getModuleSystem()->getFunctionSystem()->registerFunction("setDefaultIO", {VarType::fromString("IO")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        setDefaultIO(interpreter, arguments, return_value, object, caller_scope);
    });

    this->getModuleSystem()->getFunctionSystem()->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        IO_print(interpreter, arguments, return_value, object, caller_scope);
    });

    this->getModuleSystem()->getFunctionSystem()->registerFunction("input", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        IO_input(interpreter, arguments, return_value, object, caller_scope);
    });

    this->getModuleSystem()->getFunctionSystem()->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), IOModule::IO_Write);

    /**
     * 
     *  Writes a file to the system output such as the web client or terminal what ever is setup
     *  throws an IOException if a problem occurs
     *  function writeFile(string filename) : void
     */
    this->getModuleSystem()->getFunctionSystem()->registerFunction("writeFile", {VarType::fromString("string")}, VarType::fromString("void"), IOModule::IO_WriteFile);

    Class* c = this->getModuleSystem()->getClassSystem()->registerClass("IO");
    c->registerFunction("print", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        IO_print(interpreter, arguments, return_value, object, caller_scope);
    });

    c->registerFunction("write", {VarType::fromString("number")}, VarType::fromString("void"), IOModule::IO_Write);

    c->registerFunction("input", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        IO_input(interpreter, arguments, return_value, object, caller_scope);
    });


    c = this->getModuleSystem()->getClassSystem()->registerClass("IOPermission", this->getModuleSystem()->getClassSystem()->getClassByName("Permission"));
    c->setDescriptorObject(std::make_shared<IOPermission>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
            
    });

    // We need to override this pure function but we don't plan to do anything with it..
    c->registerFunction("__permission_check", {VarType::fromString("PermissionProperty"), VarType::fromString("PermissionProperty")}, VarType::fromString("void"), Function::Blank);

    log("IOPermission class created succesfully", LOG_LEVEL_NOTICE);


    
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
void IOModule::setDefaultIO(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    Scope* root_scope = interpreter->getCurrentScope();
    Variable* variable = root_scope->getVariable("IO");
    variable->setValue(values[0].ovalue);
}

void IOModule::IO_print(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    if (!interpreter->hasNoPermissionRestrictions())
    {
        std::shared_ptr<IOPermission> permission = std::dynamic_pointer_cast<IOPermission>(caller_scope->getPermission("IOPermission"));
        if (permission == NULL)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("PermissionException"))), "You do not have the IOPermission which is required for printing");
        }
    }
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

    interpreter->output(ss.str().c_str(), ss.str().size());
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = 1;
}

void IOModule::IO_Write(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string s;
    s += (unsigned char) values[0].dvalue;

    interpreter->output(s.c_str(), 1);
}

void IOModule::IO_WriteFile(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::ifstream fs;
    fs.open (values[0].svalue, std::fstream::in | std::fstream::binary);
    if (!fs.is_open())
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to open the file " + values[0].svalue + "for reading");
    }


    fs.seekg (0, fs.end);
    size_t size = fs.tellg();
    fs.seekg (0, fs.beg);

    char buf[1024] = {0};
    while(fs.read(buf, 1024))
    {
        interpreter->output(buf, fs.gcount());
        if (fs.gcount() != 1024 && !(fs.rdstate() & std::ifstream::eofbit))
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to write all bytes to the file " + values[0].svalue);
        }
    }

    fs.close();
    
}

void IOModule::IO_input(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string val = interpreter->input();
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = val;
}