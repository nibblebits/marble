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

#ifndef NETWORKMODULE_H
#define NETWORKMODULE_H
#include "module.h"
class Scope;
/**
 * This is the Network module and is in charge of socket communication and other network operations in marble
 */
class NetworkModule : public Module
{
public:
    NetworkModule();
    virtual ~NetworkModule();
    virtual void Init();
    void newInterpreter(Interpreter* interpreter);

    static size_t CurlReadCallback(void *contents, size_t size, size_t nmemb, void *userp);
    static size_t CurlWriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
private:

};
#endif