
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

#include "singlefunction.h"
#include "misc.h"
SingleFunction::SingleFunction(FUNCTION_TYPE type, std::string name, std::vector<VarType> argument_types, VarType return_type) : Function(type, name)
{
    this->argument_types = argument_types;
    this->return_type = return_type;
}

SingleFunction::~SingleFunction()
{
}

std::string SingleFunction::toString()
{
    std::string result = "function " + this->name + "(";
    for(VarType vtype : this->argument_types)
    {
        result += vtype.value + ",";
    }
    result = trim_right(result, ",");
    result += ") : " + this->return_type.value + ";";
    return result;
}
