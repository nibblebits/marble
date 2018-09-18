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

#include "../include/sendmailobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "networkmod.h"
#include "networkpermission.h"
#include "misc.h"
#include <stdio.h>

SendMailObject::SendMailObject(Class *c) : MailObject(c)
{
}

SendMailObject::~SendMailObject()
{
}

std::shared_ptr<Object> SendMailObject::newInstance(Class *c)
{
    return std::make_shared<SendMailObject>(c);
}

void SendMailObject::newInterpreter(Interpreter *interpreter)
{
}

void SendMailObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class SendMail extends Object
     * 
     * Allows you to send emails through Sendmail on your linux system. Requires that you have the sendmail binaries installed "/usr/lib/sendmail"
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("SendMail", moduleSystem->getClassSystem()->getClassByName("Mail"));
    c->setDescriptorObject(std::make_shared<SendMailObject>(c));

    /**
     * @class SendMail
     * 
     * Constructs this SendMail object
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), SendMailObject::SendMail_construct);
    /**
     * @class SendMail
     * 
     * Sends this email
     * 
     * function send() : void
     */
    c->registerFunction("send", {}, VarType::fromString("void"), SendMailObject::SendMail_send);
}


void SendMailObject::SendMail_construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
}


void SendMailObject::SendMail_send(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Let's check we have permission to send emails
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_EMAIL_PERMISSION_REQUIRED);

    std::shared_ptr<SendMailObject> sm_obj = std::dynamic_pointer_cast<SendMailObject>(object);

    // Let's open sendmail to send the mail
    FILE *mailpipe = popen("/usr/lib/sendmail -t", "w");
    if (mailpipe == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to open sendmail /usr/lib/sendmail do you not have sendmail installed?", interpreter->getStackTraceLog());

    fprintf(mailpipe, "%s", sm_obj->getTranscript().c_str());
    fwrite(".\n", 1, 2, mailpipe);
    pclose(mailpipe);
}
