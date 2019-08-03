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

#ifndef REFERENCEVARIABLE_H
#define REFERENCEVARIABLE_H
#include <string>
#include <memory>
#include "value.h"
#include "variable.h"
class Scope;
class Object;
class ReferenceVariable : public Variable
{
public:
    ReferenceVariable(Variable* variable_to_ref, int ref_index=-1);
    virtual ~ReferenceVariable();

    virtual void setValue(Value value, Interpreter* interpreter);
    bool isReferencingIndex();

    // The index this reference variable is pointing to in the string or array
    int ref_index;

    Variable* variable_to_ref;
};
#endif
