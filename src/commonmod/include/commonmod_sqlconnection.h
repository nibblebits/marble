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

#ifndef COMMONMOD_SQLCONNECTION
#define COMMONMOD_SQLCONNECTION

#include "object.h"
/**
 * An abstract pure SQLConnection class. Extend this if you wish to create your own SQL connection
 */

class ModuleSystem;
class CommonModule_SqlDriver;
class CommonModule_SqlConnection : public Object
{
public:
    CommonModule_SqlConnection(Class* c);
    virtual ~CommonModule_SqlConnection();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    
    static Class* registerClass(ModuleSystem* moduleSystem);


    // Native SQLConnection functions
    static void SQLConnection_Construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLConnection_getStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLConnection_getPreparedStatement(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SQLConnection_Escape(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    // The SQLDriver for this connection
    std::shared_ptr<CommonModule_SqlDriver> driver;
};

#endif