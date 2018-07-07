#include "commonmod_stringutils.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "misc.h"
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
    c->registerFunction("substr", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_substr);

    moduleSystem->getFunctionSystem()->registerFunction("substr", {VarType::fromString("string"), VarType::fromString("number"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_substr);
    moduleSystem->getFunctionSystem()->registerFunction("substr", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_substr);

    /**
     * Returns the size of the provided string
     * 
     * function strlen(string str) : number
     */
    c->registerFunction("strlen", {VarType::fromString("string")}, VarType::fromString("number"), CommonModule_StringUtils::StringUtils_strlen);
    moduleSystem->getFunctionSystem()->registerFunction("strlen", {VarType::fromString("string")}, VarType::fromString("number"), CommonModule_StringUtils::StringUtils_strlen);
    
    /**
     * Returns the position of the needle in the haystack
     * 
     * function strpos(string haystack, string needle, number pos) : number
     */
    c->registerFunction("strpos", {VarType::fromString("string"), VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("number"), CommonModule_StringUtils::StringUtils_strpos);
    moduleSystem->getFunctionSystem()->registerFunction("strpos", {VarType::fromString("string"), VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("number"), CommonModule_StringUtils::StringUtils_strpos);

    /**
     * Splits the desired string and returns an array of strings
     * 
     * function split(string target, string delim) : string[]
     */
    c->registerFunction("split", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string[]"), CommonModule_StringUtils::StringUtils_split);
    moduleSystem->getFunctionSystem()->registerFunction("split", { VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string[]"), CommonModule_StringUtils::StringUtils_split);
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

    // Is the end argument provided? if not default to 4 billion
    int end = values.size() > 2 ? values[2].dvalue  : 0xffffffff;

    if (start >= str.size())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("OutOfBoundsException"), {})));
    return_value->set(str.substr(start, end));
}

void CommonModule_StringUtils::StringUtils_strlen(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string str = values[0].svalue;
    return_value->set(str.size());
}

void CommonModule_StringUtils::StringUtils_strpos(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string haystack = values[0].svalue;
    std::string needle = values[1].svalue;
    int offset = values[2].dvalue;
    if (offset >= haystack.size())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("OutOfBoundsException"), {})));

    return_value->set(haystack.find(needle, offset));
}

void CommonModule_StringUtils::StringUtils_split(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string target = values[0].svalue;
    std::string delm = values[1].svalue;

    std::vector<std::string> splits = str_split(target, delm);
    Variable* variables = new Variable[splits.size()];
    for (int i = 0; i < splits.size(); i++)
    {
        Variable* var = &variables[i];
        var->type = VARIABLE_TYPE_STRING;
        var->value.type = VALUE_TYPE_STRING;
        var->value.holder = var;
        var->value.svalue = splits[i];
    }
    return_value->set(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, splits.size()));
}