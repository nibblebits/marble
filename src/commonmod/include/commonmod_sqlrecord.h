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

#ifndef SQLRECORD_H
#define SQLRECORD_H
#include "object.h"

/**
 * Class for a single SQL Record and it represents a record that was apart of a SQLResult 
 */

class ModuleSystem;
class CommonModule_SqlRecord : public Object
{
public:
    CommonModule_SqlRecord(Class* c);
    virtual ~CommonModule_SqlRecord();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);

    // Native SQLRecord functions
    static void SQLRecord_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLRecord_setColumn(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLRecord_getValue(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
private:
    // A map where the key is the column name and the value is the value of the column for this record instance.
    std::map<std::string, std::string> column_value;
};

#endif
