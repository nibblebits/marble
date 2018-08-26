#include "dateobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include <iostream>

DateObject::DateObject(Class* c) : Object(c)
{
    this->time = 0;
    this->format = "%d-%m-%Y %H:%M:%S";
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
    Class* c = moduleSystem->getClassSystem()->registerClass("DateFormatException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    c = moduleSystem->getClassSystem()->registerClass("Date");
    c->setDescriptorObject(std::make_shared<DateObject>(c));
    /**
     * The constructor accepts a number which represents a unix timestamp. This will be used by the date object
     */
    c->registerFunction("__construct", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        Date_ConstructWithTimestamp(interpreter, arguments, return_value, object);
    });

    c->registerFunction("setFormat", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        Date_setFormat(interpreter, arguments, return_value, object);
    });

    c->registerFunction("getFormattedString", {}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        Date_getFormattedString(interpreter, arguments, return_value, object);
    });
  
}

void DateObject::Date_ConstructWithTimestamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Invoke the parent constructor
    object->getClass()->invokeObjectParentConstructor(values, object, interpreter);

    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object); 
    date_obj->time = (time_t)values[0].dvalue;
}

void DateObject::Date_setFormat(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
     std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object); 
     date_obj->format = values[0].svalue;
}

void DateObject::Date_getFormattedString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    // Let's get the time as a formatted date string
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    char buffer[80];
    struct tm* timeinfo = localtime(&date_obj->time);
    if(strftime(buffer, sizeof(buffer), date_obj->format.c_str(), timeinfo) == 0)
    {
        // Error
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))));
    }

    std::string formatted_str = buffer;
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = formatted_str;
}