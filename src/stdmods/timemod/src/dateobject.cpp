#include "dateobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "misc.h"
#include "exceptions/systemexception.h"
#include <iostream>

DateObject::DateObject(Class *c) : Object(c)
{
    this->chosen_time = 0;
    this->format = "%d-%m-%Y %H:%M:%S";
    this->utc_hours_offset = 0;
    this->utc_minutes_offset = 0;

    // We want to start with our local timezone
    time_t our_time;
    time(&our_time);
    struct tm *timeinfo = localtime(&our_time);
    int seconds = timeinfo->tm_gmtoff;
    int minutes = (seconds / 60) % 60;
    int hours = (seconds / 60) / 60;
    this->utc_hours_offset = hours;
    this->utc_minutes_offset = minutes;
}

DateObject::~DateObject()
{
}

std::shared_ptr<Object> DateObject::newInstance(Class *c)
{
    return std::make_shared<DateObject>(c);
}

Class *DateObject::registerClass(ModuleSystem *moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("DateFormatException", moduleSystem->getClassSystem()->getClassByName("Exception"));
    /**
     * class Date
     * 
     * Respresents a given date and time. Allows formatting and the ability to get a formatted string of the specified time
     * based on the format provided.
     * 
     * Simply put this handles all Date related operations
     */
    c = moduleSystem->getClassSystem()->registerClass("Date");
    c->setDescriptorObject(std::make_shared<DateObject>(c));
    /**
     * @class Date
     * 
     * The constructor accepts a number which represents a unix timestamp. This will be used by the date object
     * function __construct(number unix_timestamp) : void
     */
    c->registerFunction("__construct", {VarType::fromString("number")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        Date_ConstructWithTimestamp(interpreter, arguments, return_value, object);
    });

    /**
     * @class Date
     * 
     * Sets the current timezone based on the timezone abbreviation that is used. For example "UTC+0:00"
     * 
     * function setTimezone(string abbreviation) : void
     */

    c->registerFunction("setTimezone", {VarType::fromString("string")}, VarType::fromString("void"), DateObject::Date_setTimezone);

    c->registerFunction("setFormat", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        Date_setFormat(interpreter, arguments, return_value, object);
    });

    c->registerFunction("getFormattedString", {}, VarType::fromString("string"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        Date_getFormattedString(interpreter, arguments, return_value, object);
    });
}

void DateObject::Date_ConstructWithTimestamp(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    // Invoke the parent constructor
    object->getClass()->invokeObjectParentConstructor(values, object, interpreter);

    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    date_obj->chosen_time = (time_t)values[0].dvalue;
}

void DateObject::Date_setTimezone(Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);

    // Let's parse the timezone to see what is going on
    std::string query = arguments[0].svalue;

    if (startsWith(query, "UTC"))
    {
        date_obj->utc_hours_offset = 0;
        date_obj->utc_minutes_offset = 0;
        // We have a UTC to parse. Let's first remove UTC
        query = str_replace(query, "UTC", "");
        if (query.size() > 0)
        {
            char op = query[0];
            query.erase(0, 1);
            std::vector<std::string> splits = str_split(query, ":");
            try
            {
                date_obj->utc_hours_offset = (op == '+') ? std::stod(splits[0]) : -std::stod(splits[0]);
                if (splits.size() > 1)
                {
                    date_obj->utc_hours_offset = (op == '+') ? std::stod(splits[0]) : -std::stod(splits[0]);
                }
            }
            catch (...)
            {
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))), "Invalid syntax. Expecting UTC+0:00 hours and minutes");
            }
        }
    }
    else
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))), "Invalid syntax. Expecting UTC+0:00 hours and minutes"); 
    }
}

void DateObject::Date_setFormat(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    date_obj->format = values[0].svalue;
}

void DateObject::Date_getFormattedString(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    // Let's get the time as a formatted date string
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);

    char buffer[80];
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_hour += date_obj->utc_hours_offset;
    timeinfo->tm_min += date_obj->utc_minutes_offset;

    time_t shiftedTime = mktime(timeinfo);
    timeinfo = gmtime(&shiftedTime);

    if (strftime(buffer, sizeof(buffer), date_obj->format.c_str(), timeinfo) == 0)
    {
        // Error
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))));
    }

    std::string formatted_str = buffer;
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = formatted_str;
}