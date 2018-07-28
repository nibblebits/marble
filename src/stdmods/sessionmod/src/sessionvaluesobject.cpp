#include "sessionvaluesobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"

SessionValuesObject::SessionValuesObject(Class *c) : Object(c)
{
}
SessionValuesObject::~SessionValuesObject()
{
}

void SessionValuesObject::registerClass(ModuleSystem *moduleSystem)
{
    Class *c = moduleSystem->getClassSystem()->registerClass("SessionValues");
    c->setDescriptorObject(std::make_shared<SessionValuesObject>(c));

    // Create an empty constructor for the SessionValues class
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * 
     * Sets the number for the index provided
     * 
     *  function setNumber(string name, number value) : number
     */
    Function *setNumber_func = c->registerFunction("setNumber", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("void"), SessionValuesObject::SessionValues_SetNumber);

    /**
     * 
     * Sets the string for the index provided
     * 
     *  function setString(string name, number value) : number
     */
    Function *setString_func = c->registerFunction("setString", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), SessionValuesObject::SessionValues_SetString);

  /**
     * 
     * Sets the object for the index provided
     * 
     *  function setObject(string name, SessionValues value) : number
     */
    Function *setObject_func = c->registerFunction("setObject", {VarType::fromString("string"), VarType::fromString("SessionValues")}, VarType::fromString("void"), SessionValuesObject::SessionValues_SetObject);

    /**
     * 
     * Gets the number for the index provided
     * 
     *  function getNumber(string name) : number
     */
    Function *getNumber_func = c->registerFunction("getNumber", {VarType::fromString("string")}, VarType::fromString("number"), SessionValuesObject::SessionValues_GetNumber);

    /**
     * 
     * Gets the string for the index provided
     * 
     *  function getString(string name) : string
     */
    Function *getString_func = c->registerFunction("getString", {VarType::fromString("string")}, VarType::fromString("string"), SessionValuesObject::SessionValues_GetString);

    /**
     * 
     * Gets the object for the index provided
     * 
     *  function getObject(string name) : SessionValues
     */
    Function *getObject_func = c->registerFunction("getObject", {VarType::fromString("string")}, VarType::fromString("SessionValues"), SessionValuesObject::SessionValues_GetObject);
}

std::shared_ptr<Object> SessionValuesObject::newInstance(Class *c)
{
    return std::make_shared<SessionValuesObject>(c);
}

void SessionValuesObject::ensureIndexExists(Interpreter* interpreter, std::string index_name)
{
    if (this->values.find(index_name) == this->values.end())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" does not exist");
}

// Native SessionValues functions/methods
void SessionValuesObject::SessionValues_SetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(object);
    std::string index_name = values[0].svalue;
    double number = values[1].dvalue;
    sv_obj->values[index_name] = number;
}

void SessionValuesObject::SessionValues_SetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(object);
    std::string index_name = values[0].svalue;
    std::string string_value = values[1].svalue;
    sv_obj->values[index_name] = string_value;
}

void SessionValuesObject::SessionValues_SetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(object);
    std::string index_name = values[0].svalue;
    std::shared_ptr<Object> obj_value = values[1].ovalue;
    sv_obj->values[index_name] = obj_value;
}

void SessionValuesObject::SessionValues_GetNumber(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_NUMBER)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not a number");

    return_value->set(sv_obj->values[index_name].dvalue);
}

void SessionValuesObject::SessionValues_GetString(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_STRING)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not a string");

    return_value->set(sv_obj->values[index_name].svalue);
}

void SessionValuesObject::SessionValues_GetObject(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<SessionValuesObject> sv_obj = std::dynamic_pointer_cast<SessionValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_OBJECT)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not a object");

    return_value->set(sv_obj->values[index_name].ovalue);
}