#include "../include/jsonobject.h"
#include "../include/jsonvaluesobject.h"
#include "exceptionobject.h"
#include "exceptions/systemexception.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "../../../commonmod/include/commonmod_value.h"
#include "function.h"
#include "misc.h"
#include "class.h"
#include "array.h"
#include <iostream>
#include <thread>

JsonObject::JsonObject(Class *c) : Object(c)
{
}

JsonObject::~JsonObject()
{
}

std::shared_ptr<Object> JsonObject::newInstance(Class *c)
{
    return std::make_shared<JsonObject>(c);
}

Value JsonObject::getNonNamedValueForJSONValue(Interpreter *interpreter, Json::Value &json_value)
{
    Json::ValueType type = json_value.type();
    switch (type)
    {
    case Json::ValueType::nullValue:
    case Json::ValueType::intValue:
    case Json::ValueType::uintValue:
    case Json::ValueType::realValue:
    case Json::ValueType::booleanValue:
    {
        return Value(json_value.asDouble());
    }
    case Json::ValueType::stringValue:
    {
        return Value(json_value.asString());
    }

    case Json::ValueType::arrayValue:
    {
        Variable *vars = new Variable[json_value.size()];
        for (int i = 0; i < json_value.size(); i++)
        {
            Value result = JsonObject::getNonNamedValueForJSONValue(interpreter, json_value[i]);
            // We must create a Value object to hold this result
            Class *value_cls = interpreter->getClassSystem()->getClassByName("Value");
            std::shared_ptr<Object> v_obj = Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Value"), {});
            Function *set_function = value_cls->getFunctionByName("set");
            Value return_val;
            set_function->invoke(interpreter, {result}, &return_val, v_obj, interpreter->getCurrentScope());
            Variable *var = &vars[i];
            var->type = VARIABLE_TYPE_OBJECT;
            var->value = v_obj;
            var->value.holder = var;
        }

        return Value(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), vars, json_value.size()));
    }

    case Json::ValueType::objectValue:
    {
        // Let's create a new JsonValues object for this Json object value
        std::shared_ptr<JsonValuesObject> jv_obj =
            std::dynamic_pointer_cast<JsonValuesObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("JsonValues"), {}));
        jv_obj->values = JsonObject::getSystemValuesForJSONValue(interpreter, json_value);
        return Value(jv_obj);
    }
    }
}

std::map<std::string, Value> JsonObject::getSystemValuesForJSONValue(Interpreter *interpreter, Json::Value &values)
{
    std::map<std::string, Value> system_values;
    Json::Value::Members member_names = values.getMemberNames();
    for (std::string name : member_names)
    {
        system_values[name] = JsonObject::getNonNamedValueForJSONValue(interpreter, values[name]);
    }

    return system_values;
}

std::string JsonObject::parseValueToJson(Value &value)
{
    std::string json_string = "";
    switch (value.type)
    {
    case VALUE_TYPE_NUMBER:
        json_string += std::to_string(value.dvalue);
        break;

    case VALUE_TYPE_STRING:
        json_string += "\"" + value.svalue + "\"";
        break;

    case VALUE_TYPE_OBJECT:
        if (value.type_str == "array")
        {
            json_string += "[";
            std::shared_ptr<Array> array_obj = std::dynamic_pointer_cast<Array>(value.ovalue);
            for (int i = 0; i < array_obj->count; i++)
            {
               std::shared_ptr<CommonModule_Value> v_obj = std::dynamic_pointer_cast<CommonModule_Value>(array_obj->variables[i].value.ovalue);
               if (v_obj == NULL)
               {
                   json_string += "null";
               }
               else
               {
                   json_string += JsonObject::parseValueToJson(v_obj->value);
               }
               json_string += ",";
            }
            json_string = trim_right(json_string, ",");
            json_string += "]";
        }
        else
        {
            std::shared_ptr<JsonValuesObject> jv_obj = std::dynamic_pointer_cast<JsonValuesObject>(value.ovalue);
            json_string += JsonObject::parseMapToJson(jv_obj->values);
        }
        break;
    }

    return json_string;
}

std::string JsonObject::parseMapToJson(std::map<std::string, Value> map)
{
    std::string json_string = "{";
    for (auto it = map.begin(); it != map.end(); it++)
    {
        std::string key = it->first;
        Value &value = it->second;
        json_string += "\"" + key + "\":";
        
        json_string += JsonObject::parseValueToJson(value);

        if (it != std::prev(map.end()))
        {
            json_string += ",";
        }
    }

    json_string += "}";

    return json_string;
}

void JsonObject::registerClass(ModuleSystem *moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("Json");
    c->setDescriptorObject(std::make_shared<JsonObject>(c));

    /**
     * @class JsonObject
     * 
     * Constructs this JsonObject object. 
     */
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class JsonObject
     * 
     * Decodes the provided JSON string into a JsonValues object
     * function decode(string json_str) : JsonValues
     */
    c->registerFunction("decode", {VarType::fromString("string")}, VarType::fromString("JsonValues"), JsonObject::Json_decode);

    /**
     * @class JsonObject
     * 
     * Encodes the provided JsonValues object into a JSON string
     * function encode(JsonValues json_values) : string
     */
    c->registerFunction("encode", {VarType::fromString("JsonValues")}, VarType::fromString("string"), JsonObject::Json_encode);
}

void JsonObject::Json_decode(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<JsonValuesObject> jv_obj =
        std::dynamic_pointer_cast<JsonValuesObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("JsonValues"), {}));
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(values[0].svalue.c_str(), root);
    if (!parsingSuccessful)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "Failed to parse JSON string", interpreter->getStackTraceLog());
    }
    jv_obj->values = JsonObject::getSystemValuesForJSONValue(interpreter, root);
    return_value->set(jv_obj);
}

void JsonObject::Json_encode(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<JsonValuesObject> jv_obj = std::dynamic_pointer_cast<JsonValuesObject>(values[0].ovalue);
    if (jv_obj == NULL)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("NullPointerException"), {})), "The provided JsonValues object is NULL", interpreter->getStackTraceLog());

    return_value->set(JsonObject::parseMapToJson(jv_obj->values));
}