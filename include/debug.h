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

#ifndef DEBUG_H
#define DEBUG_H
#include "config.h"

#ifdef DEBUG_ENABLED
#include <memory>
#include "node.h"
class Scope;
class PermissionsObject;
class Debug
{
public:
    Debug();
    virtual ~Debug();
    static void OutputTabbing(int amount);
    static void PrintValueForNode(Node* value_node, int tabbing=0);
    static void PrintVariablesForScope(Scope* scope);
    static void PrintPermissions(std::shared_ptr<PermissionsObject> permissions);
};
#endif

#endif
