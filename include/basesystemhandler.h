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

#ifndef BASESYSTEMHANDLER_H
#define BASESYSTEMHANDLER_H

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "systemhandler.h"

/**
 * 
 * This is the base SystemHandler class and it contains all base classes, functions and methods that are standard to Marble.
 * An instance of this class should be created and its class system and function system passed to the Interpreter or ModuleSystem.
 * To change the behaviour of this class override it and pass that instead.
 * 
 * Exception and Object are created in this class.
 * 
 * \attention This SystemHandler is as high as the class and function hierarchy can go.
 */ 
class BaseSystemHandler : public SystemHandler
{
public:
    BaseSystemHandler();
    virtual ~BaseSystemHandler();

private:

};

#endif