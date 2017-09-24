#include "class.h"
Class::Class(std::string name, FunctionSystem* prev_fc_sys) : FunctionSystem(prev_fc_sys)
{
    this->name = name;
    this->parent = NULL;
}

Class::Class(std::string name, Class* parent) : FunctionSystem(parent)
{
    this->name = name;
    this->parent = parent;
}

Class::~Class()
{

}

void Class::addLocalVariable(Variable v)
{
    this->local_variables.push_back(v);
}

Variable Class::getLocalVariable(std::string name)
{
    for (Variable v : this->local_variables)
    {
        if (v.name == name)
            return v;
    }
    throw std::logic_error("No local variable with name: " + name + " has been found");
}

Variable Class::getVariable(std::string name)
{
    for (Variable v : this->local_variables)
    {
        if (v.name == name)
            return v;
    }
    
    if (this->parent != NULL)
        return this->parent->getVariable(name);
        
    throw std::logic_error("No variable in this class or class parents with name: " + name + " has been found");
}

std::vector<Variable> Class::getLocalVariables()
{
    return this->local_variables;
}

std::vector<Variable> Class::getVariables()
{
    std::vector<Variable> variables = getLocalVariables();
    if (parent == NULL)
        return variables;
        
    for (Variable variable : parent->getVariables())
    {
        variables.push_back(variable);
    }
    
    return variables;
}
