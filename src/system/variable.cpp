#include "variable.h"
#include "object.h"
#include "array.h"
#include "scope.h"
#include "class.h"
#include "function.h"
#include "interpreter.h"
Variable::Variable()
{
    this->name = "";
    this->type = -1;
    this->dimensions = 0;
    this->access = MODIFIER_ACCESS_PUBLIC;
    this->scope = NULL;
    this->object = NULL;
    this->is_locked = false;
}

Variable::~Variable()
{
}

int Variable::getVariableTypeForString(std::string str)
{
    int type = VARIABLE_TYPE_OBJECT;
    if (str == "number")
    {
        type = VARIABLE_TYPE_NUMBER;
    }
    else if(str == "int")
    {
        type = VARIABLE_TYPE_NUMBER;
    }
    else if(str == "string")
    {
        type = VARIABLE_TYPE_STRING;
    }
    else if(str == "void")
    {
        type = VARIABLE_TYPE_VOID;
    }
    else if (str == "boolean" || str == "true" || str == "false")
    {
        type = VARIABLE_TYPE_BOOLEAN;
    }
    return type;
}


Variable Variable::getFromPointer(Variable* variable)
{
    Variable v;
    v.name = variable->name;
    v.type = variable->type;
    v.dimensions = variable->dimensions;
    v.type_name = variable->type_name;
    v.access = variable->access;
    v.value.set(&variable->value);
    return v;
}

bool Variable::isArray()
{
    return this->dimensions > 0;
}

void Variable::setValue(Value value, Interpreter* interpreter)
{
    if (this->is_locked)
        throw std::logic_error("This variable is locked and cannot be set");

    if (this->type == VARIABLE_TYPE_OBJECT && value.type != VALUE_TYPE_OBJECT)
    {
        // Incompatible types let's see if we have an overloaded function we can call in our object value
        if (!interpreter)
        {
            throw std::logic_error("Incompatible type while setting variable value. Pass an interpreter if you want us to try and find a compatible method in the object");
        }


        // Let's get the object class and see if it has the correct method we need
        Class* c = interpreter->getClassSystem()->getClassByName(this->type_name);
        if (c == NULL)
            throw std::logic_error("Unable to provide compatible variable value as the class of this variable type does not exist");
        
        Function* function = NULL;
        if (value.type == VALUE_TYPE_STRING)
        {
            if (c->hasOverloadedOperator("=", "string"))
            {
                function = c->getFunctionByNameAndArguments("operator:=", {VarType::fromString("string")});
            }
        }
        else if(value.type == VALUE_TYPE_NUMBER)
        {
            if (c->hasOverloadedOperator("=", "number"))
            {
                function = c->getFunctionByNameAndArguments("operator:=", {VarType::fromString("number")});
            }
        }

        if (function == NULL)
            throw std::logic_error("Could not find a function in variable object that can get the value type we are looking for");
        
        function->invoke(interpreter, {value}, &value, NULL, interpreter->getCurrentScope());
    }

    this->value = value;
    this->value.holder = this;
}

void Variable::setValue(std::string value, Interpreter* interpreter)
{
    Value v;
    v.type = VALUE_TYPE_STRING;
    v.svalue = value;
    setValue(v, interpreter);
}

void Variable::setValue(double value, Interpreter* interpreter)
{
    Value v;
    v.type = VALUE_TYPE_NUMBER;
    v.dvalue = value;
    setValue(v, interpreter);
}

void Variable::setValue(std::shared_ptr<Object> value)
{
    bool is_array = std::dynamic_pointer_cast<Array>(value) != NULL;
    if (is_array)
        throw std::logic_error("Arrays cannot currently be set using these helper methods as this functionality is not implemented");

    Value v;
    v.type = VALUE_TYPE_OBJECT;
    v.ovalue = value;
    setValue(v);
}