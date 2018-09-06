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