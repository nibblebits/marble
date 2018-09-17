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

#ifndef VARTYPE_H
#define VARTYPE_H
#include <string>
#include "statics.h"
class ClassSystem;
class VarType
{
public:
    VarType();
    virtual ~VarType();
    bool operator==(const VarType &other) const;
    bool operator!=(const VarType &other) const;
    /**
     * Ensures the compatibility between this type and the other type provided
     * \param other The type to ensure compatibility with
     * \param c_system The class system to use for checking for compatibility
     * \return Returns true if both of these types are compatible otherwise false
     */
    bool ensureCompatibility(const VarType& other, ClassSystem* c_system);

    /**
     * Returns true if this VarType represents an array
     */
    bool isArray();
    
    static VarType fromString(std::string value);
    
    VARIABLE_TYPE type;
    std::string value;
    // The array dimensions 0 if this is not an array
    int dimensions;
};

#endif
