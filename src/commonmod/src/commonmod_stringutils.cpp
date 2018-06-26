#include "commonmod_stringutils.h"
#include "function.h"
#include "variable.h"
#include "array.h"

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
    Function* f = c->registerFunction("getASCIIString", {VarType::fromString("number")}, VarType::fromString("string"), [&](Interpreter* interpreter, std::vector<Value> arguments, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope) {
        return_value->type = VALUE_TYPE_STRING;
        return_value->svalue += (char) arguments[0].dvalue;
    });

}

