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

#ifndef PROCESSMODULE_H
#define PROCESSMODULE_H
#include "module.h"
class Scope;
/**
 * This is the Process module it is responsible for all things in marble relating to processes such as shell execution
 */
class ProcessModule : public Module
{
public:
    ProcessModule();
    virtual ~ProcessModule();
    virtual void Init();
    void newInterpreter(Interpreter* interpreter);

private:

};
#endif