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

#ifndef TOKENFACTORY_H
#define TOKENFACTORY_H
#include <memory>
#include <vector>
#include "statics.h"
#include "posinfo.h"
/**
* This token factory will handle the creation of all tokens. Memory will be freed once the node factory leaves scope. Ensure that creation of tokens
* is not done from out of this class
*/

class Token;
class TokenFactory
{
public:
    TokenFactory();
    virtual ~TokenFactory();
    Token* createToken(TOKEN_TYPE token_type, PosInfo posInfo);
private:
    std::vector<std::unique_ptr<Token>> tokens;
};

#endif
