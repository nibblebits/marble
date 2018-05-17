#ifndef WEBMOD_H
#define WEBMOD_H
#include "httpd.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"

#include "module.h"
#include "class.h"
#include "object.h"

class WebModuleRequestArgumentsObject : public Object
{
public:
    WebModuleRequestArgumentsObject(Class* c);
    virtual ~WebModuleRequestArgumentsObject();

    std::map<std::string, std::string> arguments;
};

class WebModulePOSTContentObject : public Object
{
public:
    WebModulePOSTContentObject(Class* c);
    virtual ~WebModulePOSTContentObject();

    std::map<std::string, std::string> content;
};


class WebModuleObject : public Object
{
public:
    WebModuleObject(Class* c);
    virtual ~WebModuleObject();

    std::string request_uri;
    std::string requester_ip;
    std::string request_method;
    std::shared_ptr<WebModulePOSTContentObject> content;
    std::shared_ptr<WebModuleRequestArgumentsObject> request_arguments;
};

class WebModule : public Module
{
public:
    WebModule();
    virtual ~WebModule();
    void Init();
    void newInterpreter(Interpreter* interpreter);
    void parseRequest(Interpreter* interpreter, request_rec* req);
    std::map<std::string, std::string> parsePost(request_rec* req);
    std::map<std::string, std::string> parseGet(request_rec* req);
private:
};
#endif