#include "commonmod_stringutils.h"
#include "function.h"
#include "variable.h"
#include "array.h"
#include "misc.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include <iostream>

CommonModule_StringUtils::CommonModule_StringUtils(Class *c) : Object(c)
{
}

CommonModule_StringUtils::~CommonModule_StringUtils()
{
}

std::shared_ptr<Object> CommonModule_StringUtils::newInstance(Class *c)
{
    return std::make_shared<CommonModule_StringUtils>(c);
}

Class *CommonModule_StringUtils::registerClass(ModuleSystem *moduleSystem)
{
    
    /**
     * class StringUtils extends Object
     * 
     * The StringUtils class provides many methods to help manipulate strings within the marble language.
     * These methods include the ability to split strings, get the length of strings, substr and more*/
    Class *c = moduleSystem->getClassSystem()->registerClass("StringUtils");
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
    moduleSystem->getFunctionSystem()->registerFunction("split", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string[]"), CommonModule_StringUtils::StringUtils_split);

    /**
     * Replaces all instances of to_replace from the target with the replace_with string
     *
     * function str_replace(string target, string to_replace, string replace_with) : string
     */
    c->registerFunction("str_replace", {VarType::fromString("string"), VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_str_replace);
    moduleSystem->getFunctionSystem()->registerFunction("str_replace", {VarType::fromString("string"), VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_str_replace);

    /**
     * Matches all occurences in the target based on the regex string provided.
     * Returns a string array of all the occurences found
     * function preg_match_all(string target, string regex) : string[]
     */
    c->registerFunction("preg_match_all", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string[]"), CommonModule_StringUtils::StringUtils_preg_match_all);
    moduleSystem->getFunctionSystem()->registerFunction("preg_match_all", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string[]"), CommonModule_StringUtils::StringUtils_preg_match_all);

    /**
     * @class StringUtils
     * Trims whitespace from the left and right of the provided string target unless a pattern is provided
     * If a pattern is provided then the pattern is what is tripped from the string.
     *
     * function trim(string target, string pattern) : string
     */
    c->registerFunction("trim", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_trim);
    c->registerFunction("trim", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_trim);

    /**
     *
     * Trims whitespace from the left and right of the provided string target unless a pattern is provided
     * If a pattern is provided then the pattern is what is tripped from the string.
     *
     * function trim(string target, string pattern) : string
     */
    moduleSystem->getFunctionSystem()->registerFunction("trim", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_trim);
    moduleSystem->getFunctionSystem()->registerFunction("trim", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_trim);

     /**
     * @class StringUtils
     * Trims whitespace from the left of the provided string target unless a pattern is provided
     * If a pattern is provided then the pattern is what is tripped from the string.
     *
     * function trim(string target, string pattern) : string
     */
    c->registerFunction("ltrim", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_ltrim);
    c->registerFunction("ltrim", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_ltrim);

    /**
     * Trims whitespace from the left of the provided string target unless a pattern is provided
     * If a pattern is provided then the pattern is what is tripped from the string.
     *
     * function ltrim(string target, string pattern) : string
     */
    moduleSystem->getFunctionSystem()->registerFunction("ltrim", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_ltrim);
    moduleSystem->getFunctionSystem()->registerFunction("ltrim", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_ltrim);

    /**
     * @class StringUtils
     * Trims whitespace from the right of the provided string target unless a pattern is provided
     * If a pattern is provided then the pattern is what is tripped from the string.
     *
     * function rtrim(string target, string pattern) : string
     */
    c->registerFunction("rtrim", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_rtrim);
    c->registerFunction("rtrim", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_rtrim);

    /**
     * Trims whitespace from the right of the provided string target unless a pattern is provided
     * If a pattern is provided then the pattern is what is tripped from the string.
     *
     * function rtrim(string target, string pattern) : string
     */
    moduleSystem->getFunctionSystem()->registerFunction("rtrim", {VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_rtrim);
    moduleSystem->getFunctionSystem()->registerFunction("rtrim", {VarType::fromString("string"), VarType::fromString("string")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_rtrim);



    /**
     * Preforms a number format on the given target 
     *
     * function number_format(string target, number places) : string
     */
    c->registerFunction("number_format", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_number_format);
    moduleSystem->getFunctionSystem()->registerFunction("number_format", {VarType::fromString("string"), VarType::fromString("number")}, VarType::fromString("string"), CommonModule_StringUtils::StringUtils_number_format);



}

void CommonModule_StringUtils::StringUtils_getASCIIString(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    return_value->type = VALUE_TYPE_STRING;
    return_value->svalue += (char)values[0].dvalue;
}

void CommonModule_StringUtils::StringUtils_substr(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string str = values[0].svalue;
    int start = values[1].dvalue;

    // Is the end argument provided? if not default to 4 billion
    int end = values.size() > 2 ? values[2].dvalue : 0xffffffff;

    if (start >= str.size())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("OutOfBoundsException"), {})));
    return_value->set(str.substr(start, end));
}

void CommonModule_StringUtils::StringUtils_strlen(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string str = values[0].svalue;
    return_value->set(str.size());
}

void CommonModule_StringUtils::StringUtils_strpos(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string haystack = values[0].svalue;
    std::string needle = values[1].svalue;
    int offset = values[2].dvalue;
    if (offset >= haystack.size())
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("OutOfBoundsException"), {})));

    return_value->set(haystack.find(needle, offset));
}

void CommonModule_StringUtils::StringUtils_split(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string target = values[0].svalue;
    std::string delm = values[1].svalue;

    std::vector<std::string> splits = str_split(target, delm);
    Variable *variables = new Variable[splits.size()];
    for (int i = 0; i < splits.size(); i++)
    {
        Variable *var = &variables[i];
        var->type = VARIABLE_TYPE_STRING;
        var->value.type = VALUE_TYPE_STRING;
        var->value.holder = var;
        var->value.svalue = splits[i];
    }
    return_value->set(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, splits.size()));
}

void CommonModule_StringUtils::StringUtils_str_replace(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string target = values[0].svalue;
    std::string to_replace = values[1].svalue;
    std::string replace_with = values[2].svalue;

    target = str_replace(target, to_replace, replace_with);
    return_value->set(target);
}

void CommonModule_StringUtils::StringUtils_preg_match_all(Interpreter *interpreter, std::vector<Value> values, Value *return_value, std::shared_ptr<Object> object, Scope *caller_scope)
{
    std::string target = values[0].svalue;
    std::string regex = values[1].svalue;
    try
    {
        std::vector<std::string> results = preg_match_all(target, regex);
        Variable *variables = new Variable[results.size()];
        for (int i = 0; i < results.size(); i++)
        {
            Variable *var = &variables[i];
            var->type = VARIABLE_TYPE_STRING;
            var->value.type = VALUE_TYPE_STRING;
            var->value.holder = var;
            var->value.svalue = results[i];
        }
        return_value->set(std::make_shared<Array>(interpreter->getClassSystem()->getClassByName("array"), variables, results.size()));
    }
    catch (...)
    {
        throw SystemException(std::dynamic_pointer_cast<ExceptionObject>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("LogicException"), {})));
    }
}

void CommonModule_StringUtils::StringUtils_trim(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string target = values[0].svalue;
    std::string pattern = "\f\n\r\t\v";
    if (values.size() > 1)
    {
        pattern = values[1].svalue;
    }
    return_value->set(trim(target, pattern));
}

void CommonModule_StringUtils::StringUtils_ltrim(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string target = values[0].svalue;
    std::string pattern = "\f\n\r\t\v";
    if (values.size() > 1)
    {
        pattern = values[1].svalue;
    }
    return_value->set(trim_left(target, pattern));
}

void CommonModule_StringUtils::StringUtils_rtrim(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string target = values[0].svalue;
    std::string pattern = "\f\n\r\t\v";
    if (values.size() > 1)
    {
        pattern = values[1].svalue;
    }
    return_value->set(trim_right(target, pattern));
}


void CommonModule_StringUtils::StringUtils_number_format(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::string target = values[0].svalue;
    int places = values[1].dvalue;
    return_value->set(number_format(target, places));
}