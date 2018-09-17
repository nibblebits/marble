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

#ifndef COMMONMODULE_LIST
#define COMMONMODULE_LIST
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
/**
 * The List class is an abstract pure class that all classes wishing to provide List functionality should extend
 */
class CommonModule_List : public Object
{
public:
    CommonModule_List(Class* c);
    virtual ~CommonModule_List();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

};


#endif