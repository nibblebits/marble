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

#include "commonmod_sqlrecord.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"

CommonModule_SqlRecord::CommonModule_SqlRecord(Class* c) : Object(c)
{

}

CommonModule_SqlRecord::~CommonModule_SqlRecord()
{

}

std::shared_ptr<Object> CommonModule_SqlRecord::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlRecord>(c);
}
    
Class* CommonModule_SqlRecord::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLRecord");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlRecord>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlRecord::SQLRecord_Construct);

    /**
     * Sets the given column to the given value for this record.
     * function setColumn(string columnName, String recordValue) : void
     */
    c->registerFunction("setColumn", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), CommonModule_SqlRecord::SQLRecord_setColumn);
    
    /**
     * Gets the value for the given column name for this record
     * function getValue(string columnName) : string
     */
    c->registerFunction("getValue", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_SqlRecord::SQLRecord_getValue);
}


// Native SQLRecord functions
void CommonModule_SqlRecord::SQLRecord_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{

}

void CommonModule_SqlRecord::SQLRecord_setColumn(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlRecord> sql_record_obj = std::dynamic_pointer_cast<CommonModule_SqlRecord>(object);
    sql_record_obj->column_value[values[0].svalue] = values[1].svalue;
}

void CommonModule_SqlRecord::SQLRecord_getValue(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlRecord> sql_record_obj = std::dynamic_pointer_cast<CommonModule_SqlRecord>(object);
    try
    {
        std::string column_value = sql_record_obj->column_value[values[0].svalue];
        return_value->set(column_value);
    }
    catch(...)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("Exception"))), "", interpreter->getStackTraceLog());
    }
 
}