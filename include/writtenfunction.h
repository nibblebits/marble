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

#ifndef WRITTENFUNCTION_H
#define WRITTENFUNCTION_H

#include "singlefunction.h"
class SystemHandler;
class FunctionNode;
class WrittenFunction : public SingleFunction
{
 public:
        WrittenFunction(FunctionNode* function_node,  std::vector<VarType> argument_types, VarType return_type);
        virtual ~WrittenFunction();
        virtual void invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        bool hasReturned();        
        FunctionNode* getFunctionNode();
        /** The return node that caused the function to terminate. NULL if none was present.*/
        ReturnNode* return_node;
        /** The value that was returned */
        Value return_value;
        
        /* The function node related to this marble written function*/
        FunctionNode* fnode;
};
#endif
