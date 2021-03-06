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

#include "token.h"
#include "statics.h"
#include <stdlib.h>
#include <iostream>
Token::Token(int type, PosInfo posInfo)
{
    this->type = type;
    this->posInfo = posInfo;
    this->next = NULL;
}

Token::~Token()
{

}

int Token::getType()
{
    return this->type;
}

void Token::setValue(std::string svalue)
{
    this->value = svalue.c_str();
}

std::string Token::getValue()
{
    return this->value;
}

bool Token::isString()
{
    return getType() == TOKEN_TYPE_STRING;
}

bool Token::isString(std::string value)
{
    return getType() == TOKEN_TYPE_STRING && getValue() == value;
}

bool Token::isSymbol()
{
    return getType() == TOKEN_TYPE_SYMBOL;
}

bool Token::isSymbol(std::string value)
{
    return getType() == TOKEN_TYPE_SYMBOL && getValue() == value;
}

/*
* Determines if this token is a terminating symbol.
* A terminating symbol is a symbol that terminates a particular statement expression such as a semicolon. Or a function calls comma to seperate the function arguments.
*/
bool Token::isTerminatingSymbol()
{
    return isSymbol(";") || isSymbol(",");
}

bool Token::isCastableType()
{
    // The reason for that "(" is also a castable type is because of nested casting (int)(string) 32;
    return isString() || isKeyword() || isIdentifier() || isNumber() || isSymbol("(");
}

bool Token::isIdentifier()
{
    return getType() == TOKEN_TYPE_IDENTIFIER;
}

bool Token::isIdentifier(std::string value)
{
    return getType() == TOKEN_TYPE_IDENTIFIER && getValue() == value;
}

bool Token::isKeyword()
{
    return getType() == TOKEN_TYPE_KEYWORD;
}

bool Token::isKeyword(std::string value)
{
    return getType() == TOKEN_TYPE_KEYWORD && getValue() == value;
}

bool Token::isOperator()
{
    return getType() == TOKEN_TYPE_OPERATOR;
}

bool Token::isLogicalOperator()
{
    return isOperator("&&") || isOperator("||");
}

bool Token::isOperator(std::string value)
{
    return getType() == TOKEN_TYPE_OPERATOR && getValue() == value;
}

bool Token::isNumber()
{
    return getType() == TOKEN_TYPE_NUMBER;
}

bool Token::isNumber(std::string value)
{
    return getType() == TOKEN_TYPE_NUMBER && getValue() == value;
}

bool Token::isLiteral()
{
    return this->type == TOKEN_TYPE_NUMBER;
}


bool Token::isBooleanLiteral()
{
    return isKeyword("true") || isKeyword("false");
}

bool Token::isDataAccessKeyword()
{
    return isKeyword() && (value == "private" || value == "public" || value == "protected");
}


