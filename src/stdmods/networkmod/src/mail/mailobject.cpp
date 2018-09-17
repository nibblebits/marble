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

#include "../include/mailobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "../../../commonmod/include/commonmod_vector.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "networkmod.h"
#include "networkpermission.h"
#include "misc.h"

MailObject::MailObject(Class *c) : Object(c)
{

}

MailObject::~MailObject()
{
   
}

std::shared_ptr<Object> MailObject::newInstance(Class *c)
{
    return std::make_shared<MailObject>(c);
}

void MailObject::newInterpreter(Interpreter *interpreter)
{

}

std::string MailObject::getTranscript()
{
    std::string data = "";
    data += "To: " + this->to + "\r\n";
    data += "From: " + this->from + "\r\n";
    data += "Cc: " + this->cc + "\r\n";
    data += "Subject: " + this->subject + "\r\n";
    // Let's add all our extra email headers here
    for (std::string header : this->headers)
        data += header + "\r\n";
    data += "\r\n";
    data += this->body;
    return data;
}

void MailObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * pure class Mail
     * 
     * The Mail class is a pure abstract class that all mail senders should extend.
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("Mail");
    c->setDescriptorObject(std::make_shared<MailObject>(c));
    c->is_pure = true;

    /**
     * @class Mail
     * 
     * Constructs this Mail object
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class Mail
     * 
     * Sets the recipient of this email
     * 
     * function setTo(string recipient_email) : void
     */
    c->registerFunction("setTo", {VarType::fromString("string")}, VarType::fromString("void"), MailObject::Mail_setTo);

    /**
     * @class Mail
     * 
     * Sets the from address for this email
     * 
     * function setFrom(string from_email) : void
     */
    c->registerFunction("setFrom", {VarType::fromString("string")}, VarType::fromString("void"), MailObject::Mail_setFrom);

    /**
     * @class Mail
     * 
     * Sets the CC for this email
     * 
     * function setCc(string cc_email) : void
     */
    c->registerFunction("setCc", {VarType::fromString("string")}, VarType::fromString("void"), MailObject::Mail_setCc);

    /**
     * @class Mail
     * 
     * Sets the subject for this email
     * 
     * function setSubject(string subject) : void
     */
    c->registerFunction("setSubject", {VarType::fromString("string")}, VarType::fromString("void"), MailObject::Mail_setSubject);

    /**
     * @class Mail
     * 
     * Sets the body for this email
     * 
     * function setBody(string body) : void
     */
    c->registerFunction("setBody", {VarType::fromString("string")}, VarType::fromString("void"), MailObject::Mail_setBody);


    /**
     * @class Mail
     * 
     * Tells this Mail client to use a header
     * do "Content-Type: text/html; charset=us-ascii" if you wish for this email to be html.
     * Headers are appended after any Multipart file data
     * 
     * function useHeader(string header_value) : void
     */
    c->registerFunction("useHeader", {VarType::fromString("string")}, VarType::fromString("void"), MailObject::Mail_useHeader);


    /**
     * @class Mail
     * 
     * Gets the recipient of this email
     * 
     * function getTo() : string
     */
    c->registerFunction("getTo", {}, VarType::fromString("string"), MailObject::Mail_getTo);


    /**
     * @class Mail
     * 
     * Gets the sender of this email
     * 
     * function getFrom() : string
     */
    c->registerFunction("getFrom", {}, VarType::fromString("string"), MailObject::Mail_getFrom);


    /**
     * @class Mail
     * 
     * Gets the Cc of this email
     * 
     * function getCc() : string
     */
    c->registerFunction("getCc", {}, VarType::fromString("string"), MailObject::Mail_getCc);

    /**
     * @class Mail
     * 
     * Gets the subject of this email
     * 
     * function getSubject() : string
     */
    c->registerFunction("getSubject", {}, VarType::fromString("string"), MailObject::Mail_getSubject);


    /**
     * @class Mail
     * 
     * Gets the body of this email
     * 
     * function getBody() : string
     */
    c->registerFunction("getBody", {}, VarType::fromString("string"), MailObject::Mail_getBody);


    /**
     * @class Mail
     * 
     * Creates a clone of the user defined SMTP headers set for this Mail client and returns a Vector of string for those values
     * 
     * function getHeaders() : string
     */
    c->registerFunction("getHeaders", {}, VarType::fromString("string"), MailObject::Mail_getHeaders);


    /**
     * @class Mail
     * 
     * Returns the email string formatted to how it should be sent directly to the SMTP server.
     * It includes SMTP headers along with the message body
     * 
     * function getTranscript() : string
     */
    c->registerFunction("getTranscript", {}, VarType::fromString("string"), MailObject::Mail_getTranscript);


    /**
     * @class Mail
     * 
     * Sends the email. This method should be overrided by any email handler
     * pure function send() : void
     */
    Function* send_func = c->registerFunction("send", {}, VarType::fromString("void"), Function::Blank);
    // We expect an override.
    send_func->is_pure = true;

}


void MailObject::Mail_setTo(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    mail_obj->to = values[0].svalue;
}

void MailObject::Mail_setFrom(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    mail_obj->from = values[0].svalue;
}

void MailObject::Mail_setCc(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    mail_obj->cc = values[0].svalue;  
}

void MailObject::Mail_setSubject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    mail_obj->subject = values[0].svalue;  
}

void MailObject::Mail_setBody(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    mail_obj->body = values[0].svalue;  
}

void MailObject::Mail_useHeader(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    mail_obj->headers.push_back(values[0].svalue);
}

void MailObject::Mail_getTo(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    return_value->set(mail_obj->to);
}

void MailObject::Mail_getFrom(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    return_value->set(mail_obj->from);
}

void MailObject::Mail_getCc(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    return_value->set(mail_obj->cc);
}

void MailObject::Mail_getSubject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    return_value->set(mail_obj->subject);
}

void MailObject::Mail_getBody(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    return_value->set(mail_obj->body);  
}

void MailObject::Mail_getHeaders(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    std::shared_ptr<CommonModule_Vector> vector = std::dynamic_pointer_cast<CommonModule_Vector>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Vector"), {}));
    for(std::string v : mail_obj->headers)
        vector->vec_values.push_back(v);

    return_value->set(vector);
}

void MailObject::Mail_getTranscript(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<MailObject> mail_obj = std::dynamic_pointer_cast<MailObject>(object);
    return_value->set(mail_obj->getTranscript());
}