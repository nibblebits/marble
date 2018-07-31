#include "filesessionobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
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


std::map<std::string, Value> FileSessionObject::getSystemValuesForJSONValue(Interpreter* interpreter, Json::Value& values)
{
    std::map<std::string, Value> system_values;
    Json::Value::Members member_names = values.getMemberNames();
    for (std::string name : member_names)
    {
        Json::Value json_value = values[name];
        Json::ValueType type = json_value.type();
        switch(type)
        {
            case Json::ValueType::nullValue:
            case Json::ValueType::intValue:
            case Json::ValueType::uintValue:
            case Json::ValueType::realValue:
            case Json::ValueType::booleanValue:
                system_values[name] = Value(json_value.asDouble());
            break;
        
            case Json::ValueType::stringValue:
                 system_values[name] = Value(json_value.asString());
            break;

            case Json::ValueType::arrayValue:
            
            break;

            case Json::ValueType::objectValue:
                // Let's create a new SystemValues object for this Json object value
                std::shared_ptr<SessionValuesObject> sv_obj = 
                        std::dynamic_pointer_cast<SessionValuesObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("SessionValues"), {}));
                sv_obj->values = FileSessionObject::getSystemValuesForJSONValue(interpreter, json_value);
                system_values[name] = Value(sv_obj);
            break;
        }
    }

    return system_values;
}

void FileSessionObject::FileSession_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileSessionObject> fs_obj = std::dynamic_pointer_cast<FileSessionObject>(object);
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

std::string FileSessionObject::parseMapToJson(std::map<std::string, Value> map)
{
    std::string new_session_json = "{";
    for (auto it = map.begin(); it != map.end(); it++)
    {
        std::string key = it->first;
        Value& value = it->second;
        new_session_json += "\"" + key + "\":";
        switch(value.type)
        {
            case VALUE_TYPE_NUMBER:
                new_session_json += std::to_string(value.dvalue);
            break;

            case VALUE_TYPE_STRING:
                new_session_json += "\"" + value.svalue + "\"";
            break;

            case VALUE_TYPE_OBJECT:
                std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(value.ovalue);
                new_session_json += FileSessionObject::parseMapToJson(sv_obj->values);
            break;
        }

        if (it != std::prev(map.end()))
        {
            new_session_json += ",";
        }
    }

    new_session_json += "}";

    return new_session_json;
}

void FileSessionObject::FileSession_Save(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileSessionObject> fs_obj = std::dynamic_pointer_cast<FileSessionObject>(object);
    std::string new_session_json = FileSessionObject::parseMapToJson(fs_obj->values);


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