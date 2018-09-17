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

#ifndef COMMONMOD_PREPAREDSTATEMENT
#define COMMONMOD_PREPAREDSTATEMENT

#include "commonmod_sqlstatement.h"
#include <deque>
class ModuleSystem;
/**
 * Class for Prepared SQL statements
 */
class CommonModule_PreparedStatement : public CommonModule_SqlStatement
{
public:
    CommonModule_PreparedStatement(Class* c);
    virtual ~CommonModule_PreparedStatement();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);


    // Native PreparedStatement functions
    static void PreparedStatement_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void PreparedStatement_finalizeQuery(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void PreparedStatement_addParameter(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
   
    // The provided values of this prepared statement, these are the values to be escaped
    std::deque<std::string> values;
};

#endif