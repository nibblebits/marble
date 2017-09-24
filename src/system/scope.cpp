#include "scope.h"
#include "object.h"
#include "array.h"
#include <stdexcept>
#include <memory>

Scope::Scope()
{
    this->prev = NULL;
}
Scope::~Scope()
{
}

void Scope::registerVariable(Variable* variable)
{
    if (variable == NULL)
    {
        throw std::logic_error("NULL variables are not allowed");
    }
    this->variables.push_back(variable);
}

Variable* Scope::createVariable()
{
    Variable* variable = new Variable();
    this->unique_variables.push_back(std::unique_ptr<Variable>(variable));
    registerVariable(variable);
    return variable;
}

Variable* Scope::getVariable(std::string variable_name)
{
    for (Variable* variable : this->variables)
    {
        if (variable->name == variable_name)
        {
            return variable;
        }
    }

    return NULL;
}

std::vector<Variable*> Scope::getVariables()
{
    return this->variables;
}

std::vector<Variable*> Scope::getObjectVariablesFor(std::shared_ptr<Object> object)
{
    std::vector<Variable*> variables;
    for (Variable* variable : getVariables())
    {
        Value value = variable->value;
        if (value.type == VALUE_TYPE_OBJECT 
            || value.type == VALUE_TYPE_ARRAY)
        {
            std::shared_ptr<Object> v_obj = value.ovalue;
            if (value.type == VALUE_TYPE_OBJECT)
            {
                throw std::logic_error("OBJECTS NOT YET SUPPORTED");
            }
            
            if(object == v_obj)
                variables.push_back(variable);
        }
    }
    return variables;
}
