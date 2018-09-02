#include "commonmod_value.h"
#include "variable.h"
#include "value.h"
#include "array.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"


CommonModule_Value::CommonModule_Value(Class* c) : Object(c)
{

}
CommonModule_Value::~CommonModule_Value()
{

}
std::shared_ptr<Object> CommonModule_Value::newInstance(Class* c)
{
    return std::make_shared<CommonModule_Value>(c);
}

Class* CommonModule_Value::registerClass(ModuleSystem* moduleSystem)
{
 /**
     * class Value
     * 
     * The Value class holds either a string, number, boolean or Object 
     */ 
    Class* c = moduleSystem->getClassSystem()->registerClass("Value");
    c->setDescriptorObject(std::make_shared<CommonModule_Value>(c));

    Function* constructor = c->registerFunction("__construct", {}, VarType::fromString("void"), Function::Blank);

    /**
     * function set(string value) : void
     * 
     * Set's this Value object to have a string value
     */
    Function* set_function = c->registerFunction("set", {VarType::fromString("string")}, VarType::fromString("void"), Value_Set);
    
    /**
     * function set(number value) : void
     * 
     * Set's this Value object to have a number value
     */
    set_function = c->registerFunction("set", {VarType::fromString("number")}, VarType::fromString("void"), Value_Set);

    /**
     * function set(Object value) : void
     * 
     * Set's this Value object to have an Object value
     */
    set_function = c->registerFunction("set", {VarType::fromString("Object")}, VarType::fromString("void"), Value_Set);

    /**
     * function operator:=(string value) : Value
     * 
     * Allows string assignments to this Value object
     */
    Function* equal_operator_func = c->registerFunction("operator:=", {VarType::fromString("string")}, VarType::fromString("Value"), Value_AssignmentSet);
    equal_operator_func->is_operator_overloading = true;
    equal_operator_func->overloaded_operator = "=";

    /**
     * function operator:=(number value) : Value
     * 
     * Allows number assignments to this Value object
     */
    equal_operator_func = c->registerFunction("operator:=", {VarType::fromString("number")}, VarType::fromString("Value"), Value_AssignmentSet);
    equal_operator_func->is_operator_overloading = true;
    equal_operator_func->overloaded_operator = "=";

    /**
     * function operator:=(Object value) : Value
     * 
     * Allows Object assignments to this Value object
     */
    equal_operator_func = c->registerFunction("operator:=", {VarType::fromString("Object")}, VarType::fromString("Value"), Value_AssignmentSet);
    equal_operator_func->is_operator_overloading = true;
    equal_operator_func->overloaded_operator = "=";


    /**
     * function getString() : string
     * 
     * Get's this values string value alias of toString()
     */
    Function* getString_function = c->registerFunction("getString", {}, VarType::fromString("string"), Value_GetString);

    /**
     * function toString() : string
     * 
     * Get's this values string value alias of getString()
     */
    Function* toString_function = c->registerFunction("toString", {}, VarType::fromString("string"), Value_GetString);



    /**
     * function getNumber() : string
     * 
     * Get's this values number value alias of toNumber()
     */
    Function* getNumber_function = c->registerFunction("getNumber", {}, VarType::fromString("number"), Value_GetNumber);


    /**
     * function toNumber() : number
     * 
     * Get's this values number value alias of getNumber()
     */
    Function* toNumber_function = c->registerFunction("toNumber", {}, VarType::fromString("number"), Value_GetNumber);



    /**
     * function getObject() : Object
     * 
     * Get's this values Object value
     */
    Function* getObject_function = c->registerFunction("getObject", {}, VarType::fromString("Object"), Value_GetObject);


}


void CommonModule_Value::Value_AssignmentSet(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Value> new_value_obj
                     = std::dynamic_pointer_cast<CommonModule_Value>(Object::create(interpreter, interpreter->getClassSystem()->getClassByName("Value"), {}));
    values[0].holder = NULL;
    new_value_obj->value = values[0];
    return_value->set(new_value_obj);
}

void CommonModule_Value::Value_Set(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Value> vobject = std::dynamic_pointer_cast<CommonModule_Value>(object);
    values[0].holder = NULL;
    vobject->value = values[0];
}

void CommonModule_Value::Value_GetString(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Value> vobject = std::dynamic_pointer_cast<CommonModule_Value>(object);
    if (vobject->value.type == VALUE_TYPE_NUMBER)
        vobject->value.svalue = std::to_string(vobject->value.dvalue);
    return_value->set(vobject->value.svalue);
}

void CommonModule_Value::Value_GetNumber(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Value> vobject = std::dynamic_pointer_cast<CommonModule_Value>(object);
    return_value->set(vobject->value.dvalue);  
}

void CommonModule_Value::Value_GetObject(Interpreter* interpreter, std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object, Scope* caller_scope)
{
    std::shared_ptr<CommonModule_Value> vobject = std::dynamic_pointer_cast<CommonModule_Value>(object);
    return_value->set(vobject->value.ovalue);  
}