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

#include "commonmod_sqlresult.h"
#include "modulesystem.h"
#include "commonmod_sqlrecord.h"

CommonModule_SqlResult::CommonModule_SqlResult(Class* c) : Object(c)
{

}

CommonModule_SqlResult::~CommonModule_SqlResult()
{

}

std::shared_ptr<Object> CommonModule_SqlResult::newInstance(Class* c)
{
    return std::make_shared<CommonModule_SqlResult>(c);
}
    
Class* CommonModule_SqlResult::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("SQLResult");
    c->setDescriptorObject(std::make_shared<CommonModule_SqlResult>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), CommonModule_SqlResult::SQLResult_Construct);
    
    /**
     * @class SQLResult
     * Adds a SQLRecord to this SQLResult object
     * function addRecord(SQLRecord record) : void
     */
    c->registerFunction("addRecord", {VarType::fromString("SQLRecord")}, VarType::fromString("void"), CommonModule_SqlResult::SQLResult_addRecord);

    /**
     * @class SQLResult
     * Returns the next record available otherwise returns NULL
     * 
       function getNextRecord() : SQLRecord
     */
    c->registerFunction("getNextRecord", {}, VarType::fromString("SQLRecord"), CommonModule_SqlResult::SQLResult_getNextRecord);

    /**
     * @class SQLResult
     * Returns the total number of records available
     */
    c->registerFunction("count", {}, VarType::fromString("number"), CommonModule_SqlResult::SQLResult_Count);
}


// Native SQLResult functions
void CommonModule_SqlResult::SQLResult_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{

}

void CommonModule_SqlResult::SQLResult_addRecord(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlResult> sql_result_obj = std::dynamic_pointer_cast<CommonModule_SqlResult>(object);
    sql_result_obj->records.push_back(std::dynamic_pointer_cast<CommonModule_SqlRecord>(values[0].ovalue));
}

void CommonModule_SqlResult::SQLResult_getNextRecord(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
   std::shared_ptr<CommonModule_SqlResult> sql_result_obj = std::dynamic_pointer_cast<CommonModule_SqlResult>(object);
   
   if (sql_result_obj->records.empty())
   {
       // We are empty lets return a NULL object
       return_value->type = VALUE_TYPE_OBJECT;
       return_value->ovalue = NULL;
       return;
   }

   // Let's return the next record, we start at the front
   return_value->set(sql_result_obj->records.front());
   // Remove the front from the vector
   sql_result_obj->records.pop_front();

}

void CommonModule_SqlResult::SQLResult_Count(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_SqlResult> sql_result_obj = std::dynamic_pointer_cast<CommonModule_SqlResult>(object);
    return_value->set(sql_result_obj->records.size());
}