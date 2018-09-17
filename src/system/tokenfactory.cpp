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

#include "tokenfactory.h"
#include "token.h"
TokenFactory::TokenFactory()
{

}
TokenFactory::~TokenFactory()
{

}

Token* TokenFactory::createToken(TOKEN_TYPE token_type, PosInfo posInfo)
{
    Token* token;
    switch(token_type)
    {
    case TOKEN_TYPE_NUMBER:
    {
        token = new Token(TOKEN_TYPE_NUMBER, posInfo);
    }
    break;

    case TOKEN_TYPE_OPERATOR:
    {
        token = new Token(TOKEN_TYPE_OPERATOR, posInfo);
    }
    break;

    case TOKEN_TYPE_STRING:
    {
        token = new Token(TOKEN_TYPE_STRING, posInfo);
    }
    break;

    case TOKEN_TYPE_IDENTIFIER:
    {
        token = new Token(TOKEN_TYPE_IDENTIFIER, posInfo);
    }
    break;

    case TOKEN_TYPE_KEYWORD:
    {
        token = new Token(TOKEN_TYPE_KEYWORD, posInfo);
    }
    break;

    case TOKEN_TYPE_SYMBOL:
    {
        token = new Token(TOKEN_TYPE_SYMBOL, posInfo);
    }
    break;

    default:
        throw std::logic_error("Invalid token type provided to TokenFactory::createToken");
    }

    // Lets create a unique pointer and add it to the nodes vector so the memory is freed when the node factory leaves its scope.
    this->tokens.push_back(std::unique_ptr<Token>(token));
    return token;
}
