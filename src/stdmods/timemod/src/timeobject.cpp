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

#include "timeobject.h"
#include "modulesystem.h"
#include <sstream>
#include <time.h>
TimeObject::TimeObject(Class* c) : Object(c)
{

}

TimeObject::~TimeObject()
{

}

std::shared_ptr<Object> TimeObject::newInstance(Class* c)
{
    return std::make_shared<TimeObject>(c);
}

Class* TimeObject::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class Time
     * 
     * Handles time operations
     */
    Class* c = moduleSystem->getClassSystem()->registerClass("Time");
    /**
     * @class Time
     * 
     * Returns the current unix time
     * function getTimeStamp() : number
     */
    c->registerFunction("getTimeStamp", {}, VarType::fromString("number"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        Time_getTimeStamp(interpreter, arguments, return_value, object);
    });
}

void TimeObject::Time_getTimeStamp(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)
{
    time_t t = time(0);
    return_value->type = VALUE_TYPE_NUMBER;
    return_value->dvalue = (double) t;
}