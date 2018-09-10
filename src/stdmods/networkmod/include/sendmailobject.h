#ifndef SENDMAILOBJECT_H
#define SENDMAILOBJECT_H

#include "mailobject.h"


class ModuleSystem;
class Interpreter;
class SendMailObject : public MailObject
{
public:
    SendMailObject(Class* c);
    virtual ~SendMailObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);    
    static void newInterpreter(Interpreter* interpreter);

    // Native SendMail methods
    static void SendMail_construct(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void SendMail_send(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);





};
#endif