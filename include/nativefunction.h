
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

#ifndef NATIVEFUNCTION_H
#define NATIVEFUNCTION_H

#include "singlefunction.h"
#include "statics.h"
#include "typedef_func.h"
class NativeFunction : public SingleFunction
{
    public:
        NativeFunction(std::string name, std::vector<VarType> argument_types, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint);
        virtual ~NativeFunction();
        virtual void invoke_impl(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    private:
        NATIVE_FUNCTION_ENTRYPOINT entrypoint;
};

#endif
