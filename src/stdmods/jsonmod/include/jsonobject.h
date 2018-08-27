#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include "object.h"
#include <jsoncpp/json/json.h>
#include <map>
#include <string>


class ModuleSystem;
class JsonObject : public Object
{
public:
    JsonObject(Class* c);
    virtual ~JsonObject();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Value getNonNamedValueForJSONValue(Interpreter* interpreter, Json::Value& json_value);
    static std::map<std::string, Value> getSystemValuesForJSONValue(Interpreter* interpreter, Json::Value& values);
    static std::string parseValueToJson(Value& value);
    static std::string parseMapToJson(std::map<std::string, Value> map);
    
    static void registerClass(ModuleSystem* moduleSystem);

    // Native Json methods
    static void Json_decode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Json_encode(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

};
#endif