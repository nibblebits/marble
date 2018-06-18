#ifndef WEBMOD_H
#define WEBMOD_H
#include "httpd.h"
#include "apache_mod.h"
#include "http_config.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"
#include "apache_mod.h"
#include "module.h"
#include "class.h"
#include "object.h"

class MultipartFileObject : public Object
{
public:
    MultipartFileObject(Class* c);
    virtual ~MultipartFileObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);
    std::string name;
    std::string ext;
    std::string type;
    std::string path;
};

// The structure for multipart parses
struct multipart_parse
{
    // The standard post content excluding files, key = field name, value = field value
    std::map<std::string, std::string> post_field_content;
    
    // File content for multipart file uploads, key = field name, value = multipart file
    std::map<std::string, std::shared_ptr<MultipartFileObject>> file_field_content;

    // Multipart file uploads whose filename is an array get mapped here
    std::map<std::string, std::vector<std::shared_ptr<MultipartFileObject>> > file_field_content_array;

};

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

class WebModulePOSTFileContentObject : public Object
{
public:
    WebModulePOSTFileContentObject(Class* c);
    virtual ~WebModulePOSTFileContentObject();

    // Multipart file uploads whoe filename is singular get mapped here
    std::map<std::string, std::shared_ptr<MultipartFileObject>> content;

    // Multipart file uploads whose filename is an array get mapped here
    std::map<std::string, std::vector<std::shared_ptr<MultipartFileObject>> > content_array;
};


class WebModuleObject : public Object
{
public:
    WebModuleObject(Class* c);
    virtual ~WebModuleObject();

    // The given apache request object
    request_rec* req;
    
    std::string request_uri;
    std::string requester_ip;
    std::string request_method;
    std::shared_ptr<WebModulePOSTContentObject> content;
    std::shared_ptr<WebModulePOSTFileContentObject> file_content;
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

    std::string readNextInBuffer(char* buf_end, char** ptr, std::string delm);

    /**
     * Parses the given string into a key and value map. Each element ends when delm is spotted
     * for example "str" may equal "name=abc; age=21;"
     * 
     * Operation ends when end of line is reached
     */
    std::map<std::string, std::string> parseKeyAndValueForString(std::string str);
    /**
     * Extracts a given value from a string. For example you may get a string such as
     * "hello world \" this is a test"
     * 
     * and this function will see the qoutes at the start and remove them and remove the escape sequence character from
     * within the string
     */
    std::string extractValueFromString(std::string str);

    /**
     * Parses the multipart header info provided and returns a map where the key is the header name such as Content-Type
     * and the value is a map of attributes for this header value.
     * 
     * For example
     * 
     * Content-Type: text/html
     * 
     * This will result in the following map
     * 
     * Key: Content-Type
     * Value: 
     *       Key: text/html
     *       Value:
     *
     * 
     * Another example
     * 
     * Content-Disposition: form-data; name="field_name"
     * Key: Content-Disposition
     * Value:
     *      Key: form-data
     *      Value:
     *      Key: name
     *      Value: field_name
     */
    std::map<std::string, std::map<std::string, std::string>> handleMultipartHeaderInfo(std::string header_info);

    struct multipart_parse parseMultipartFormData(request_rec* req, Interpreter* interpreter);
    std::map<std::string, std::string> parsePost(request_rec* req);
    std::map<std::string, std::string> parseGet(request_rec* req);

    // Static methods to be called in the marble language
    static void Request_setResponseHeader(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Request_getFileContent(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FileContent_has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FileContent_get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FileContent_hasArray(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FileContent_getArray(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getType(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getName(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getPath(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void MultipartFile_getExtension(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

private:

};
#endif