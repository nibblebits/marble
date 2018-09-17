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

#include "purefunction.h"
PureFunction::PureFunction(std::string function_name, std::vector<VarType> argument_types, VarType return_type) : SingleFunction(FUNCTION_TYPE_PURE, function_name, argument_types, return_type)
{
    this->is_pure = true;
}

PureFunction::~PureFunction()
{

}

void PureFunction::invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    throw std::logic_error("Pure functions cannot be invoked directly. The validator should have stopped this so this is a bug please report it.");
}