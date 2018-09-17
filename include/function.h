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

#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
#include "statics.h"
#include "typedef_func.h"

class ReturnNode;
class SystemHandler;
class Class;
class Function
{
    public:
        Function(FUNCTION_TYPE type, std::string name);
        virtual ~Function();
        virtual void invoke(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) = 0;
        std::string getName();
        
        /** The Class that this function is a method of. Set to NULL if this is not a method just a function with no class. */
        Class* cls; 
        FUNCTION_TYPE type;
        std::string name;
        bool is_pure;

        /**
         * is_operator_overloading should be true if the function is an overloaded operator
         * the overloaded_operator string should be set to the operator that is overloaded
         */
        bool is_operator_overloading;
        std::string overloaded_operator;

        // Is this a private, protected or public variable?
        MODIFIER_ACCESS access;


        // An empty static function used for when a native programmer wants to create a function that points to nothing.
        static void Blank(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
};
#endif
