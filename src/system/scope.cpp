#include "scope.h"
#include <stdexcept>

Scope::Scope()
{

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
