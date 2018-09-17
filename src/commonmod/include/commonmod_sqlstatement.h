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

#ifndef COMMONMOD_SQLSTATEMENT
#define COMMONMOD_SQLSTATEMENT

#include "object.h"
#include <string>
class ModuleSystem;
class CommonModule_SqlConnection;

/**
 * Class for SQL statements
 */
class CommonModule_SqlStatement : public Object
{
public:
    CommonModule_SqlStatement(Class* c);
    virtual ~CommonModule_SqlStatement();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);


    // Native SQLStatement functions
    static void SQLStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLStatement_Execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLStatement_setQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLStatement_finalizeQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
protected:
    // The sql connection for this SQLStatement
    std::shared_ptr<CommonModule_SqlConnection> connection;

    // The query for this SqlStatement
    std::string query;
};

#endif