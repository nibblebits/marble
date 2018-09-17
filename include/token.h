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

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>
#include "posinfo.h"

class Token
{
public:
    Token(int type, PosInfo posInfo);
    virtual ~Token();
    void setType(int type);
    int getType();
    void setValue(std::string svalue);
    std::string getValue();

    bool isString();
    bool isString(std::string value);
    bool isSymbol();
    bool isSymbol(std::string value);
    bool isTerminatingSymbol();
    bool isCastableType();
    bool isIdentifier();
    bool isIdentifier(std::string value);
    bool isKeyword();
    bool isKeyword(std::string value);
    bool isOperator();
    bool isLogicalOperator();
    bool isOperator(std::string value);
    bool isNumber();
    bool isNumber(std::string value);
    bool isLiteral();
    bool isBooleanLiteral();
    bool isDataAccessKeyword();

    // The next token after this one
    Token* next;
    int type;
    std::string value;
    
    PosInfo posInfo;
private:
};
#endif
