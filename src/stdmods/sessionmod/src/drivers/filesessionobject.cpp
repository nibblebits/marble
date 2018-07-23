#include "filesessionobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>
#include <fstream>

FileSessionObject::FileSessionObject(Class* c) : SessionObject(c)
{

}

FileSessionObject::~FileSessionObject()
{

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


}

std::shared_ptr<Object> FileSessionObject::newInstance(Class* c)
{
    return std::make_shared<FileSessionObject>(c);
}


std::map<std::string, Value> FileSessionObject::getSystemValuesForJSONValue(Json::Value& values)
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

            break;
        }
    }

    return system_values;
}

void FileSessionObject::FileSession_Create(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<FileSessionObject> fs_obj = std::dynamic_pointer_cast<FileSessionObject>(object);
    std::fstream file;
    file.open("./testing_json.txt", std::ios::out | std::ios::in);
    Json::Reader reader;
    Json::Value obj;
    Json::Value::Members members;
    try
    {
        file >> obj;
        fs_obj->values = FileSessionObject::getSystemValuesForJSONValue(obj);
        
    }
    catch(...)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "Failed to load or create session");
    }

}
