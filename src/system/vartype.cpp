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

#include "vartype.h"
#include "variable.h"
#include "csystem.h"
#include "class.h"
#include "misc.h"
#include <iostream>
VarType::VarType()
{
    this->type = -1;
    this->value = "";
    this->dimensions = 0;
}

VarType::~VarType()
{

}

bool VarType::operator==(const VarType &other) const
{
    if (this->type != other.type)
        return false;
    if (this->value != other.value)
        return false;
    
    return true;
}

bool VarType::operator!=(const VarType &other) const
{
    if (this->type == other.type && this->value == other.value)
        return false;
    return true;
}


bool VarType::ensureCompatibility(const VarType& other, ClassSystem* c_system)
{   
    if (this->type == VARIABLE_TYPE_OBJECT && other.type == VARIABLE_TYPE_OBJECT)
    {
        Class* f1_type_cls = c_system->getClassByName(this->value);
        Class* f2_type_cls = c_system->getClassByName(other.value);
        if (f1_type_cls == NULL || f2_type_cls == NULL)
            return false;

        if (!f1_type_cls->instanceOf(f2_type_cls))
        {
            return false;
        }
        return true;
    }
    else
    {
        return this->type == other.type;
    }
}

bool VarType::isArray()
{
    return this->dimensions > 0;
}

VarType VarType::fromString(std::string value)
{
    std::vector<std::string> value_split = str_split(value, "[]");
    VarType type;
    type.value = value_split[0];
    type.dimensions = value_split.size()-1;
    type.type = Variable::getVariableTypeForString(value_split[0]);
    return type;
}
