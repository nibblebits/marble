#ifndef CURLOBJECT_H
#define CURLOBJECT_H

#include "object.h"
#include <curl/curl.h>

class ModuleSystem;
class Interpreter;
class CurlObject : public Object
{
public:
    CurlObject(Class* c);
    virtual ~CurlObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static void registerClass(ModuleSystem* moduleSystem);    
    static void newInterpreter(Interpreter* interpreter);

    CURL* curl;

    // This is where data is written to after an execution
    std::string write_data;

    // Native CurlObject methods
    static void Curl_setopt(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Curl_execute(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Curl_close(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};
#endif