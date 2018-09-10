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
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter->getClassSystem()->getClassByName("IOException"))), "Failed to open sendmail /usr/lib/sendmail do you not have sendmail installed?");

    fprintf(mailpipe, "%s", sm_obj->getTranscript().c_str());
    fwrite(".\n", 1, 2, mailpipe);
    pclose(mailpipe);
}
