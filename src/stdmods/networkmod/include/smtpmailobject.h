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

#ifndef SMTPMAILOBJECT_H
#define SMTPMAILOBJECT_H

#include "mailobject.h"
#include <curl/curl.h>


class ModuleSystem;
class Interpreter;
class SmtpMailObject : public MailObject
{
public:
    SmtpMailObject(Class* c);
    virtual ~SmtpMailObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);    
    static void newInterpreter(Interpreter* interpreter);

    std::string host;
    unsigned short port;
    bool use_ssl;
    std::string username;
    std::string password;

    CURL* curl;
    
    // Native SmtpMail methods
    static void SmtpMail_construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SmtpMail_setHost(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SmtpMail_setPort(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SmtpMail_setUseSSL(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SmtpMail_setUsername(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SmtpMail_setPassword(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SmtpMail_send(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);





};
#endif