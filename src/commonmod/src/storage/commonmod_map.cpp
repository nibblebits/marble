#include "commonmod_map.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "misc.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>

CommonModule_Map::CommonModule_Map(Class* c) : Object(c)
{

}
CommonModule_Map::~CommonModule_Map()
{

}
std::shared_ptr<Object> CommonModule_Map::newInstance(Class* c)
{
    return std::make_shared<CommonModule_Map>(c);
}

Class* CommonModule_Map::registerClass(ModuleSystem* moduleSystem)
{
    /**
     * class Map
     * 
     * The Map class provides map functionality and allows Value's to be set or accessed through an index name
     */ 
    Class* c = moduleSystem->getClassSystem()->registerClass("Map");
    c->setDescriptorObject(std::make_shared<CommonModule_Map>(c));
    c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * @class Map
     * function set(string index, Value value) : void
     * 
     * Sets the index value represented by the index to the new provided value
     */
    c->registerFunction("set", {VarType::fromString("string"), VarType::fromString("Value")}, VarType::fromString("void"), CommonModule_Map::Map_Set);

    /**
     * @class Map
     * function get(string index) : Value
     * 
     * Gets the Value from this map that has the index name that is equal to the index provided
     */
    c->registerFunction("get", {VarType::fromString("string")}, VarType::fromString("Value"), CommonModule_Map::Map_Get);
}

void CommonModule_Map::Map_Set(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Map> map_obj = std::dynamic_pointer_cast<CommonModule_Map>(object);
    map_obj->value_map[values[0].svalue] = std::make_unique<Value>(&values[1]);
}

void CommonModule_Map::Map_Get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Map> map_obj = std::dynamic_pointer_cast<CommonModule_Map>(object);
    if (map_obj->value_map.find(values[0].svalue) == map_obj->value_map.end())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("InvalidIndexException"), {})), "This map does not contain the key \"" + values[0].svalue + "\"", interpreter->getStackTraceLog());

    Value* value_map_value = map_obj->value_map[values[0].svalue].get();
    Value return_val = Value(value_map_value);
    return_value->set(&return_val);
}