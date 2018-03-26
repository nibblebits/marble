#include "dateobject.h"
#include "modulesystem.h"
#include <iostream>

DateObject::DateObject(Class* c) : Object(c)
{
    this->time = 0;
}

DateObject::~DateObject()
{

}

std::shared_ptr<Object> DateObject::newInstance(Class* c)
{
    return std::make_shared<DateObject>(c);
}

Class* DateObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("Date");

    /**
     * The constructor accepts a number which represents a unix timestamp. This will be used by the date object
     */
    c->registerFunction("__construct", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        Date_ConstructWithTimestamp(interpreter, arguments, return_value, object);
    });
}

void DateObject::Date_ConstructWithTimestamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Invoke the parent constructor
    object->invokeParentConstructor(interpreter, values);

    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object); 
    std::cout << "DATE OBJECT: " << date_obj << std::endl;
    date_obj->time = 55;
}

void DateObject::Date_getFormattedString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Let's get the time as a formatted date string
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    char buffer[80];
    struct tm* timeinfo = localtime(&date_obj->time);
    if(strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo) == 0)
    {
        // Error
        throw std::logic_error("problem");
    }

    std::string formatted_str = buffer;
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = formatted_str;
}