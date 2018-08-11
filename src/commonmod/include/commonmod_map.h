#ifndef COMMONMODULE_MAP
#define COMMONMODULE_MAP
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <string>
#include <memory>
/**
 * The Map class represents a class that has map functionality, the ability to set values by an index name and get values by an index name
 */

class Value;
class CommonModule_Map : public Object
{
public:
    CommonModule_Map(Class* c);
    virtual ~CommonModule_Map();
    virtual std::shared_ptr<Object> newInstance(Class* c);

    static Class* registerClass(ModuleSystem* moduleSystem);

    static void Map_Set(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);
    static void Map_Get(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope);

    // This is the C++ map that holds Value's for the Marble Map
    std::map<std::string, std::unique_ptr<Value>> value_map;

};


#endif