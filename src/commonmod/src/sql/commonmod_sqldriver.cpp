#include "commonmod_sqldriver.h"
#include "commonmod_sqlstatement.h"
#include "commonmod_preparedstatement.h"
#include "modulesystem.h"
#include "function.h"
#include "interpreter.h"
#include "exceptions/systemexception.h"
#include <iostream>
CommonModule_SqlDriver::CommonModule_SqlDriver(Class* c) : Object(c)
{

}

CommonModule_SqlDriver::~CommonModule_SqlDriver()
{

}

std::shared_ptr<Object> CommonModule_SqlDriver::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlDriver>(c);
}
    
Class* CommonModule_SqlDriver::registerClass(ModuleSystem* moduleSystem)
{
    // Register a driver not found exception, thrown when a driver does not exist.
    Class* exception = moduleSystem->getClassSystem()->registerClass("DriverNotFoundException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    exception->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    Class* c = moduleSystem->getClassSystem()->registerClass("SQLDriver");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlDriver>(c));
    c->is_pure = true;

    // function __construct() : void
    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlDriver::SQLDriver_Construct);
    // function connect(string host, string username, string password, string database) : SQLConnection
    Function* connect_function = c->registerFunction("connect", {VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string"),
                                    VarType::fromString("string")},
                                    VarType::fromString("SQLConnection"), Function::Blank);

    connect_function->is_pure = true;

    // function registerDriver(SQLDriver driver) : void
    c->registerFunction("registerDriver", {VarType::fromString("SQLDriver")}, VarType::fromString("void"), CommonModule_SqlDriver::SQLDriver_registerDriver);

    // function getDriver(string driver_name) : SQLDriver
    c->registerFunction("getDriver", {VarType::fromString("string")}, VarType::fromString("SQLDriver"), CommonModule_SqlDriver::SQLDriver_getDriver);

    // function execute(SQLConnection connection, SQLStatement statement, string finalized_query) : void
    Function* execute = c->registerFunction("execute", {VarType::fromString("SQLConnection"), VarType::fromString("SQLStatement"), VarType::fromString("string")}, VarType::fromString("void"), Function::Blank);
    execute->is_pure = true;
}

void CommonModule_SqlDriver::newInterpreter(Interpreter* interpreter)
{
    // Create a variable named SQLDriver that just points to an object instance of SQLDriver
    interpreter->getCurrentScope()->createVariable("SQLDriver", "SQLDriver", Object::create(interpreter->getClassSystem()->getClassByName("SQLDriver")));
}

// Native SQLDriver functions
void CommonModule_SqlDriver::SQLDriver_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    // Invoke the parent constructor
    object->getClass("SQLDriver")->invokeObjectParentConstructor(values, object, interpreter);
}

void CommonModule_SqlDriver::SQLDriver_registerDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlDriver> driver_obj = std::dynamic_pointer_cast<CommonModule_SqlDriver>(object);
    std::shared_ptr<CommonModule_SqlDriver> passed_driver_obj = std::dynamic_pointer_cast<CommonModule_SqlDriver>(values[0].ovalue);
    driver_obj->drivers.push_back(passed_driver_obj); 
}

void CommonModule_SqlDriver::SQLDriver_getDriver(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlDriver> driver_obj = std::dynamic_pointer_cast<CommonModule_SqlDriver>(object);
    std::shared_ptr<CommonModule_SqlDriver> selected_driver = NULL;
    for (std::shared_ptr<CommonModule_SqlDriver> _driver : driver_obj->drivers)
    {
        // Is this _driver's class name equal to the class name of the driver we are trying to get
        if (_driver->getClass()->name == values[0].svalue)
        {
            selected_driver = _driver;
            break;
        }
    }

    if (selected_driver == NULL)
        throw SystemException(Object::create(interpreter->getClassSystem()->getClassByName("DriverNotFoundException")));

    // We have a driver, lets now return it to the user
    return_value->set(selected_driver);

}