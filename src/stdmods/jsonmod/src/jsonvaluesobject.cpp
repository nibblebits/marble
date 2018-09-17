/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "jsonvaluesobject.h"
#include "modulesystem.h"
#include "interpreter.h"
#include "function.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"

JsonValuesObject::JsonValuesObject(Class *c) : Object(c)
{
}

JsonValuesObject::~JsonValuesObject()
{
}

void JsonValuesObject::registerClass(ModuleSystem *moduleSystem)
{
    /**
     * class JsonValues
     * 
     * The JsonValues is returned after parsing Json succesfully. Each JsonValues object acts as a map 
     * where it can hold multiple parsed json values by name and value.
     */
    Class *c = moduleSystem->getClassSystem()->registerClass("JsonValues");
    c->setDescriptorObject(std::make_shared<JsonValuesObject>(c));

    // Create an empty constructor for the JsonValues class
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);


    /**
     * @class JsonValues
     * Returns true if an index with the given name exists
     * 
     *  function has(string index_name) : boolean
     */
    Function *has_func = c->registerFunction("has", {VarType::fromString("string")}, VarType::fromString("boolean"), JsonValuesObject::JsonValues_Has);


    /**
     * @class JsonValues
     * Sets the number for the index provided
     * 
     *  function setNumber(string name, number value) : number
     */
    Function *setNumber_func = c->registerFunction("setNumber", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("void"), JsonValuesObject::JsonValues_SetNumber);

    /**
     * @class JsonValues
     * Sets the string for the index provided
     * 
     *  function setString(string name, number value) : number
     */
    Function *setString_func = c->registerFunction("setString", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("void"), JsonValuesObject::JsonValues_SetString);

  /**
     * @class JsonValues
     * Sets the object for the index provided
     * 
     *  function setObject(string name, SessionValues value) : number
     */
    Function *setObject_func = c->registerFunction("setObject", {VarType::fromString("string"), VarType::fromString("JsonValues")}, VarType::fromString("void"), JsonValuesObject::JsonValues_SetObject);

    /**
     * @class JsonValues
     * Gets the number for the index provided
     * 
     *  function getNumber(string name) : number
     */
    Function *getNumber_func = c->registerFunction("getNumber", {VarType::fromString("string")}, VarType::fromString("number"), JsonValuesObject::JsonValues_GetNumber);

    /**
     * @class JsonValues
     * Gets the string for the index provided
     * 
     *  function getString(string name) : string
     */
    Function *getString_func = c->registerFunction("getString", {VarType::fromString("string")}, VarType::fromString("string"), JsonValuesObject::JsonValues_GetString);

    /**
     * @class JsonValues
     * Gets the object for the index provided
     * 
     *  function getObject(string name) : SessionValues
     */
    Function *getObject_func = c->registerFunction("getObject", {VarType::fromString("string")}, VarType::fromString("JsonValues"), JsonValuesObject::JsonValues_GetObject);

    /**
     * @class JsonValues
     * Gets the array for the index provided
     * 
     *  function getArray(string name) : array
     */
    Function *getArray_func = c->registerFunction("getArray", {VarType::fromString("string")}, VarType::fromString("Value[]"), JsonValuesObject::JsonValues_GetArray);

}

std::shared_ptr<Object> JsonValuesObject::newInstance(Class *c)
{
    return std::make_shared<JsonValuesObject>(c);
}

void JsonValuesObject::ensureIndexExists(Interpreter* interpreter, std::string index_name)
{
    if (this->values.find(index_name) == this->values.end())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" does not exist", interpreter->getStackTraceLog());
}


void JsonValuesObject::JsonValues_Has(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    return_value->set(sv_obj->values.find(values[0].svalue) != sv_obj->values.end());
}

void JsonValuesObject::JsonValues_SetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    double number = values[1].dvalue;
    sv_obj->values[index_name] = number;
}

void JsonValuesObject::JsonValues_SetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    std::string string_value = values[1].svalue;
    sv_obj->values[index_name] = string_value;
}

void JsonValuesObject::JsonValues_SetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    std::shared_ptr<Object> obj_value = values[1].ovalue;
    sv_obj->values[index_name] = obj_value;
}

void JsonValuesObject::JsonValues_GetNumber(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_NUMBER)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not a number", interpreter->getStackTraceLog());

    return_value->set(sv_obj->values[index_name].dvalue);
}

void JsonValuesObject::JsonValues_GetString(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_STRING)
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not a string", interpreter->getStackTraceLog());

    return_value->set(sv_obj->values[index_name].svalue);
}

void JsonValuesObject::JsonValues_GetObject(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_OBJECT || v.type_str == "array")
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not a object", interpreter->getStackTraceLog());

    return_value->set(sv_obj->values[index_name].ovalue);
}

void JsonValuesObject::JsonValues_GetArray(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::shared_ptr<JsonValuesObject> sv_obj = std::dynamic_pointer_cast<JsonValuesObject>(object);
    std::string index_name = values[0].svalue;
    sv_obj->ensureIndexExists(interpreter, index_name);

    Value &v = sv_obj->values[index_name];
    if (v.type != VALUE_TYPE_OBJECT || v.type_str != "array")
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("IOException"), {})), "The index with the name \"" + index_name + "\" is not an array", interpreter->getStackTraceLog());

    return_value->set(sv_obj->values[index_name].ovalue);
}
