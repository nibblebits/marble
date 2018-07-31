#ifndef FILESESSIONOBJECT_H
#define FILESESSIONOBJECT_H

#include "sessionobject.h"
#include "value.h"
#include <jsoncpp/json/json.h>
#include <vector> 
#include <fstream>

#define TMP_DIRECTORY "/var/tmp"

class ModuleSystem;
class FileSessionObject : public SessionObject
{
public:
    FileSessionObject(Class* c);
    virtual ~FileSessionObject();

    static void registerClass(ModuleSystem* moduleSystem);
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static std::map<std::string, Value> getSystemValuesForJSONValue(Interpreter* interpreter, Json::Value& values);


    std::fstream file;

    static std::string parseMapToJson(std::map<std::string, Value> map);
    
    // Native FileSession functions/methods
    static void FileSession_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void FileSession_Save(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};

#endif