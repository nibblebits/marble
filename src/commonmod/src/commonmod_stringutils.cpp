#include "commonmod_stringutils.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>

CommonModule_StringUtils::CommonModule_StringUtils(Class* c) : Object(c)
{
  
}

CommonModule_StringUtils::~CommonModule_StringUtils()
{

}

std::shared_ptr<Object> CommonModule_StringUtils::newInstance(Class* c)
{
    return std::make_shared<CommonModule_StringUtils>(c);
}

Class* CommonModule_StringUtils::registerClass(ModuleSystem* moduleSystem)
{
    Class* c = moduleSystem->getClassSystem()->registerClass("StringUtils");
    c->setDescriptorObject(std::make_shared<CommonModule_StringUtils>(c));
    c->registerFunction("getASCIIString", {VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_getASCIIString);
    
    /**
     * Returns a part of a string based on the position and total
     * 
     * function substr(string str, number pos, number size) : string
     */
    c->registerFunction("substr", {VarType::fromString("string"), VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_substr);
    moduleSystem->getFunctionSystem()->registerFunction("substr", {VarType::fromString("string"), VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_substr);

    /**
     * Returns the size of the provided string
     * 
     * function strlen(string str) : number
     */
    c->registerFunction("strlen", {VarType::fromString("string")}, VarType::fromString("number"), CommonModule_StringUtils::StringUtils_strlen);
    moduleSystem->getFunctionSystem()->registerFunction("strlen", {VarType::fromString("string")}, VarType::fromString("number"), CommonModule_StringUtils::StringUtils_strlen);
    
    
}



void CommonModule_StringUtils::StringUtils_getASCIIString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue += (char) values[0].dvalue;
}

void CommonModule_StringUtils::StringUtils_substr(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string str = values[0].svalue;
    int start = values[1].dvalue;
    int end = values[2].dvalue;

    if (start >= str.size() || end >= str.size())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("OutOfBoundsException"), {})));
    return_value->set(str.substr(start, end));
}

void CommonModule_StringUtils::StringUtils_strlen(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string str = values[0].svalue;
    return_value->set(str.size());
}