#include "../include/smtpmailobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "networkmod.h"
#include "networkpermission.h"
#include "misc.h"
SmtpMailObject::SmtpMailObject(Class *c) : MailObject(c)
{
    this->port = 587;
    this->curl = NULL;
}

SmtpMailObject::~SmtpMailObject()
{
    // Clean up if the user failed to do so
    if (this->curl != NULL)
        curl_easy_cleanup(this->curl);
}

std::shared_ptr<Object> SmtpMailObject::newInstance(Class *c)
{
    return std::make_shared<SmtpMailObject>(c);
}

void SmtpMailObject::newInterpreter(Interpreter *interpreter)
{
}

void SmtpMailObject::registerClass(ModuleSystem *moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("SmtpMail", moduleSystem->getClassSystem()->getClassByName("Mail"));
    c->setDescriptorObject(std::make_shared<SmtpMailObject>(c));

    /**
     * @class SmtpMail
     * 
     * Constructs this SmtpMail object
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), SmtpMailObject::SmtpMail_construct);

    /**
     * @class SmtpMail
     * 
     * Sets the host of the SMTP server
     * 
     * function setHost(string hostname) : void
     */
    c->registerFunction("setHost", {VarType::fromString("string")}, VarType::fromString("void"), SmtpMailObject::SmtpMail_setHost);

    /**
     * @class SmtpMail
     * 
     * Sets the port of the SMTP server
     * 
     * function setPort(number port) : void
     */
    c->registerFunction("setPort", {VarType::fromString("number")}, VarType::fromString("void"), SmtpMailObject::SmtpMail_setPort);

    /**
     * @class SmtpMail
     * 
     * Set this to true if we should use SSL to communicate with this SMTP server
     * 
     * function setUseSSL(boolean use_ssl) : void
     */
    c->registerFunction("setUseSSL", {VarType::fromString("boolean")}, VarType::fromString("void"), SmtpMailObject::SmtpMail_setUseSSL);

    /**
     * @class SmtpMail
     * 
     * Sets the username for this SMTP server
     * 
     * function setUsername(string username) : void
     */
    c->registerFunction("setUsername", {VarType::fromString("string")}, VarType::fromString("void"), SmtpMailObject::SmtpMail_setUsername);

    /**
     * @class SmtpMail
     * 
     * Sets the password for this SMTP server
     * 
     * function setPassword(string password) : void
     */
    c->registerFunction("setPassword", {VarType::fromString("string")}, VarType::fromString("void"), SmtpMailObject::SmtpMail_setPassword);

    /**
     * @class SmtpMail
     * 
     * Sends this email
     * 
     * function send() : void
     */
    c->registerFunction("send", {}, VarType::fromString("void"), SmtpMailObject::SmtpMail_send);
}

void SmtpMailObject::SmtpMail_construct(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    smtp_mail_obj->curl = curl_easy_init();
}
void SmtpMailObject::SmtpMail_setHost(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    smtp_mail_obj->host = values[0].svalue;
}

void SmtpMailObject::SmtpMail_setPort(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    smtp_mail_obj->port = (unsigned short)values[0].dvalue;
}

void SmtpMailObject::SmtpMail_setUseSSL(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    smtp_mail_obj->use_ssl = values[0].dvalue;
}

void SmtpMailObject::SmtpMail_setUsername(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    smtp_mail_obj->username = values[0].svalue;

    // No from provided? Then we provide the username as the from field
    if (smtp_mail_obj->from == "")
    {
        smtp_mail_obj->from = smtp_mail_obj->username;
    }
}

void SmtpMailObject::SmtpMail_setPassword(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    smtp_mail_obj->password = values[0].svalue;
}

void SmtpMailObject::SmtpMail_send(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    // Let's check we have permission to send emails
    NetworkPermission::ensurePermission(interpreter, caller_scope, NETWORK_PERMISSION_EMAIL_PERMISSION_REQUIRED);
    
    /*
     * We now need to tell curl to send our email
     */
    struct curl_slist *recipients = NULL;
    std::shared_ptr<SmtpMailObject> smtp_mail_obj = std::dynamic_pointer_cast<SmtpMailObject>(object);
    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_USERNAME, smtp_mail_obj->username.c_str());
    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_PASSWORD, smtp_mail_obj->password.c_str());

    if (smtp_mail_obj->use_ssl)
    {
        curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_URL, ("smtps://" + smtp_mail_obj->host + ":" + std::to_string(smtp_mail_obj->port)).c_str());
        curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    }
    else
    {
        curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_URL, ("smtp://" + smtp_mail_obj->host + ":" + std::to_string(smtp_mail_obj->port)).c_str());
        curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_USE_SSL, (long)CURLUSESSL_NONE);
    }
    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_MAIL_FROM, smtp_mail_obj->from.c_str());

    recipients = curl_slist_append(recipients, smtp_mail_obj->to.c_str());
    if (smtp_mail_obj->cc != "")
    {
        recipients = curl_slist_append(recipients, smtp_mail_obj->cc.c_str());
    }

    std::string final_data = smtp_mail_obj->getTranscript();

    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_MAIL_RCPT, recipients);
    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_READFUNCTION, NetworkModule::CurlReadCallback);
    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_READDATA, final_data);
    curl_easy_setopt(smtp_mail_obj->curl, CURLOPT_UPLOAD, 1L);

    // Send the message
    CURLcode res = curl_easy_perform(smtp_mail_obj->curl);

    /* Check for errors */
    if (res != CURLE_OK)
    {
        std::cout << "failed here throw exception for future purposes: " << curl_easy_strerror(res) << std::endl;
    }

    /* Free the list of recipients */
    curl_slist_free_all(recipients);
}