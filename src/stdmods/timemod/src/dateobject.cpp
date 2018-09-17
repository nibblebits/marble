/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

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

    /**
     * @class Date
     * 
     * Sets the format for this date
     * 
     * %a 	Abbreviated weekday name 	Sun
        %A 	Full weekday name 	Sunday
        %b 	Abbreviated month name 	Mar
        %B 	Full month name 	March
        %c 	Date and time representation 	Sun Aug 19 02:56:02 2012
        %d 	Day of the month (01-31) 	19
        %H 	Hour in 24h format (00-23) 	14
        %I 	Hour in 12h format (01-12) 	05
        %j 	Day of the year (001-366) 	231
        %m 	Month as a decimal number (01-12) 	08
        %M 	Minute (00-59) 	55
        %p 	AM or PM designation 	PM
        %S 	Second (00-61) 	02
        %U 	Week number with the first Sunday as the first day of week one (00-53) 	33
        %w 	Weekday as a decimal number with Sunday as 0 (0-6) 	4
        %W 	Week number with the first Monday as the first day of week one (00-53) 	34
        %x 	Date representation 	08/19/12
        %X 	Time representation 	02:50:06
        %y 	Year, last two digits (00-99) 	01
        %Y 	Year 	2012
        %Z 	Timezone name or abbreviation 	CDT
        %% 	A % sign 	%

        Example: %d-%M-%Y may equal 01-01-2018 if the date points to this the time 01-01-2018.

        Use getFormattedString() to get the date in the format you provided here
     * function setFormat(string format) : void
     */
    c->registerFunction("setFormat", {VarType::fromString("string")}, VarType::fromString("void"), [&](Interpreter *interpreter, std::vector<Value> arguments, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope) {
        Date_setFormat(interpreter, arguments, return_value, object);
    });

    /**
     * @class Date
     * Returns this date as a formatted string based on the format you specified with setFormat
     * function getFormattedString() : string
     */
    c->registerFunction("getFormattedString", {}, VarType::fromString("string"), DateObject::Date_getFormattedString);

    /**
     * @class Date
     * Sets the current hour for this date
     * Setting the hour over the amount of hours left of this day will cause the time to roll over to the next day
     * function setHour(number hour) : void
     */
    c->registerFunction("setHour", {VarType::fromString("number")}, VarType::fromString("void"), Date_setHour);

    /**
     * @class Date
     * Sets the current minute for this date
     * Setting the minutes over the amount of minutes left of this hour will cause the time to roll over to the next hour
     * function setMinute(number hour) : void
     */
    c->registerFunction("setMinute", {VarType::fromString("number")}, VarType::fromString("void"), Date_setMinute);

    /**
     * @class Date
     * Sets the current second for this date
     * Setting the seconds over the amount of seconds of this minute left will cause the time to roll over to the next minute
     * function setSecond(number second) : void
     */
    c->registerFunction("setSecond", {VarType::fromString("number")}, VarType::fromString("void"), Date_setSecond);

    /**
     * @class Date
     * Sets the current day for this date.
     * Setting over the current amount of days for that month will cause the time to rollover to the following month
     * function setDay(number day) : void
     */
    c->registerFunction("setDay", {VarType::fromString("number")}, VarType::fromString("void"), Date_setDay);

    /**
     * @class Date
     * Sets the current month for this date
     * Setting the current amount of months over the current year will cause the time to rollover to the following year
     * function setMonth(number month) : void
     */
    c->registerFunction("setMonth", {VarType::fromString("number")}, VarType::fromString("void"), Date_setMonth);

    /**
     * @class Date
     * Sets the current year for this date.
     * function setYear(number year) : void
     */
    c->registerFunction("setYear", {VarType::fromString("number")}, VarType::fromString("void"), Date_setYear);

    /**
     * @class Date
     * Gets the current hour for this date
     * function getHour() : number
     */
    c->registerFunction("getHour", {}, VarType::fromString("number"), Date_getHour);

    /**
     * @class Date
     * Gets the current minute for this date
     * function getMinute() : number
     */
    c->registerFunction("getMinute", {}, VarType::fromString("number"), Date_getMinute);

    /**
     * @class Date
     * Gets the current second for this date
     * function getSecond() : number
     */
    c->registerFunction("getSecond", {}, VarType::fromString("number"), Date_getSecond);

    /**
     * @class Date
     * Gets the current day for this date
     * function getDay() : number
     */
    c->registerFunction("getDay", {}, VarType::fromString("number"), Date_getDay);

    /**
     * @class Date
     * Gets the current month for this date
     * function getMonth() : number
     */
    c->registerFunction("getMonth", {}, VarType::fromString("number"), Date_getMonth);

    /**
     * @class Date
     * Gets the current year for this date
     * function getYear() : number
     */
    c->registerFunction("getYear", {}, VarType::fromString("number"), Date_getYear);
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
                throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))), "Invalid syntax. Expecting UTC+0:00 hours and minutes", interpreter->getStackTraceLog());
            }
        }
    }
    else
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))), "Invalid syntax. Expecting UTC+0:00 hours and minutes", interpreter->getStackTraceLog());
    }
}

void DateObject::Date_setFormat(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    date_obj->format = values[0].svalue;
}

void DateObject::Date_getFormattedString(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("DateFormatException"))), "", interpreter->getStackTraceLog());
    }

    std::string formatted_str = buffer;
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue = formatted_str;
}

void DateObject::Date_setHour(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_hour = values[0].dvalue;
    date_obj->chosen_time = mktime(timeinfo);
    // Disable the hours offset since we have changed the hour
    date_obj->utc_hours_offset = 0;
}

void DateObject::Date_setMinute(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_min = values[0].dvalue;
    date_obj->chosen_time = mktime(timeinfo);
    // Disable the minutes offset since we have changed the hour
    date_obj->utc_minutes_offset = 0;
}

void DateObject::Date_setSecond(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_sec = values[0].dvalue;
    date_obj->chosen_time = mktime(timeinfo);
}

void DateObject::Date_setDay(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_mday = values[0].dvalue;
    date_obj->chosen_time = mktime(timeinfo);
}

void DateObject::Date_setMonth(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    // -1 as C documentation says January starts at zero
    timeinfo->tm_mon = values[0].dvalue - 1;
    date_obj->chosen_time = mktime(timeinfo);
}

void DateObject::Date_setYear(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    // Doc says we must start at 1900
    timeinfo->tm_year = values[0].dvalue - 1900;
    date_obj->chosen_time = mktime(timeinfo);
}

void DateObject::Date_getHour(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_hour += date_obj->utc_hours_offset;
    timeinfo->tm_min += date_obj->utc_minutes_offset;
    return_value->set(timeinfo->tm_hour);
}

void DateObject::Date_getMinute(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    timeinfo->tm_hour += date_obj->utc_hours_offset;
    timeinfo->tm_min += date_obj->utc_minutes_offset;
    return_value->set(timeinfo->tm_hour);
}

void DateObject::Date_getSecond(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    return_value->set(timeinfo->tm_sec);
}

void DateObject::Date_getDay(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    return_value->set(timeinfo->tm_mday);
}

void DateObject::Date_getMonth(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    return_value->set(timeinfo->tm_mon);
}

void DateObject::Date_getYear(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<DateObject> date_obj = std::dynamic_pointer_cast<DateObject>(object);
    struct tm *timeinfo = gmtime(&date_obj->chosen_time);
    // Doc says we must start at the year 1900
    return_value->set(1900 + timeinfo->tm_year);
}

