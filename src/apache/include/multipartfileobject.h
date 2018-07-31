#ifndef WEBMODULE_MULTIPARTFILEOBJECT
#define WEBMODULE_MULTIPARTFILEOBJECT
#include "object.h"
#include "httpd.h"
#include "apache_mod.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"
#include "module.h"
#include "class.h"
#include <stdio.h>
#include <string>
#include <memory>
class MultipartFileObject : public Object
{
public:
    MultipartFileObject(Class* c);
    virtual ~MultipartFileObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    /**
     *
     * Registers the RequestArguments class
     */
    static Class* registerClass(ModuleSystem* moduleSystem);

    // MultipartFile properties
    std::string name;
    std::string ext;
    std::string type;
    std::string path;

    // Static Native RequestArgument functions
    static void MultipartFile_getType(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getName(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getPath(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getExtension(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);


};


#endif