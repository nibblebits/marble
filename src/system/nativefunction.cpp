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

#include "nativefunction.h"
NativeFunction::NativeFunction(std::string name, std::vector<VarType> argument_types, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint) : SingleFunction(FUNCTION_TYPE_NATIVE, name, argument_types, return_type)
{
    this->entrypoint = entrypoint;
}
NativeFunction::~NativeFunction()
{

}
void NativeFunction::invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    if (this->entrypoint == NULL)
    {
        throw std::logic_error("Cannot invoke function " + this->name + " because it has no entrypoint");
    }
    entrypoint(interpreter, values, return_value, object, caller_scope);
}
