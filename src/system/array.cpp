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

#include "array.h"
#include "variable.h"
#include "interpreter.h"
#include <iostream>

Array::Array(Class* c) : Object(c)
{
    this->variables = NULL;
    this->count = 0;
}

Array::Array(Class* c, Variable* variables, int count) : Object(c)
{
    this->variables = variables;
    this->count = count;
}


Array::~Array()
{
   if (this->variables != NULL)
      delete[] variables;
}

void Array::setup()
{
    // Nothing to setup
}

