#include "filesessionobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "../../../commonmod/include/commonmod_value.h"
#include "array.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "misc.h"
#include <iostream>

FileSessionObject::FileSessionObject(Class* c) : SessionObject(c)
{

}

FileSessionObject::~FileSessionObject()
{
    this->file.close();
}

void FileSessionObject::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("FileSession", moduleSystem->getClassSystem()->getClassByName("Session"));
    c->setDescriptorObject(std::make_shared<FileSessionObject>(c));

    // Create an empty constructor for the FileSession class
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * 
     * Creates or loads the session with the provided key.
     * function create(string session_key) : void
     */
    Function *create_func = c->registerFunction("create", {VarType::fromString("string")}, VarType::fromString("void"), FileSessionObject::FileSession_Create);

    /**
     * 
     * Saves the current session.
     * This method is called before destruction of a Session object
     * function save() : void
     */
    Function* save_func = c->registerFunction("save", {}, VarType::fromString("void"), FileSessionObject::FileSession_Save);


}

std::shared_ptr<Object> FileSessionObject::newInstance(Class* c)
{
    return std::make_shared<FileSessionObject>(c);
}



Value FileSessionObject::getNonNamedValueForJSONValue(Interpreter *interpreter, Json::Value &json_value)
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
            Value result = FileSessionObject::getNonNamedValueForJSONValue(interpreter, json_value[i]);
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
        std::shared_ptr<SessionValuesObject> jv_obj =
            std::dynamic_pointer_cast<SessionValuesObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("SessionValues"), {}));
        jv_obj->values = FileSessionObject::getSystemValuesForJSONValue(interpreter, json_value);
        return Value(jv_obj);
    }
    }
}

std::map<std::string, Value> FileSessionObject::getSystemValuesForJSONValue(Interpreter *interpreter, Json::Value &values)
{
    std::map<std::string, Value> system_values;
    Json::Value::Members member_names = values.getMemberNames();
    for (std::string name : member_names)
    {
        system_values[name] = FileSessionObject::getNonNamedValueForJSONValue(interpreter, values[name]);
    }

    return system_values;
}

std::string FileSessionObject::parseValueToJson(Value &value)
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
                   json_string += FileSessionObject::parseValueToJson(v_obj->value);
               }
               json_string += ",";
            }
            json_string = trim_right(json_string, ",");
            json_string += "]";
            std::cout << json_string << std::endl;
        }
        else
        {
            std::shared_ptr<SessionValuesObject> jv_obj = std::dynamic_pointer_cast<SessionValuesObject>(value.ovalue);
            json_string += FileSessionObject::parseMapToJson(jv_obj->values);
        }
        break;
    }

    return json_string;
}

std::string FileSessionObject::parseMapToJson(std::map<std::string, Value> map)
{
    std::string json_string = "{";
    for (auto it = map.begin(); it != map.end(); it++)
    {
        std::string key = it->first;
        Value &value = it->second;
        json_string += "\"" + key + "\":";
        
        json_string += FileSessionObject::parseValueToJson(value);

        if (it != std::prev(map.end()))
        {
            json_string += ",";
        }
    }

    json_string += "}";

    return json_string;
}

void FileSessionObject::FileSession_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileSessionObject> fs_obj = std::dynamic_pointer_cast<FileSessionObject>(object);
   
    // Invoke the parent class Create method
    fs_obj->getClass()->parent->getFunctionByNameAndArguments("create", {VarType::fromString("string")})->invoke(interpreter, values, return_value, object, caller_scope);

    if (interpreter->properties.find("session_key") == interpreter->properties.end())
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "Cannot create Session as no session key has been setup for this client");
    }

    std::string session_key = interpreter->properties["session_key"];
    std::string session_password = values[0].svalue;
    std::string path = std::string(TMP_DIRECTORY) + "/" + "marble_session_" + session_key + "_" + session_password + ".txt";
    bool exists = file_exists(path);
    if (exists)
    {
        fs_obj->file.open(path, std::ios::out | std::ios::in);
        Json::Reader reader;
        Json::Value obj;
        Json::Value::Members members;
        try
        {
            fs_obj->file >> obj;
            fs_obj->values = FileSessionObject::getSystemValuesForJSONValue(interpreter, obj);
        }
        catch(...)
        {
            throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "Failed to load or create session");
        }
    }
    else
    {
        // This is a new session
        fs_obj->file.open(path, std::ios::out);
        fs_obj->file.write("{}", 2);
    }


}

void FileSessionObject::FileSession_Save(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileSessionObject> fs_obj = std::dynamic_pointer_cast<FileSessionObject>(object);
    std::string new_session_json = FileSessionObject::parseMapToJson(fs_obj->values);

    if (!fs_obj->file.is_open())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "Failed to save session as you never created it. Call create(string session_password)");

    // Reset to the start of the stream
    fs_obj->file.clear();
    fs_obj->file.seekg(0, std::ios::end);
    int file_length = fs_obj->file.tellg();

    // Ignore all characters in the stream currently
    fs_obj->file.ignore(file_length);
    fs_obj->file.seekg(0, std::ios::beg);

    // Save the session
    fs_obj->file.write(new_session_json.c_str(), new_session_json.size());
    fs_obj->file.flush();

}