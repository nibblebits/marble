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

#ifndef SINGLEFUNCTION_H
#define SINGLEFUNCTION_H

#include <vector>
#include <memory>
#include "value.h"
#include "object.h"
#include "vartype.h"
#include "function.h"

class SingleFunction : public Function
{
    public:
        SingleFunction(FUNCTION_TYPE type, std::string name, std::vector<VarType> argument_types, VarType return_type);
        virtual ~SingleFunction();
        virtual std::string toString();
        std::vector<VarType> argument_types;
        VarType return_type;
};
#endif
