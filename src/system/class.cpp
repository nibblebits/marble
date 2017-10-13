#include "class.h"
Class::Class(Interpreter* interpreter, std::string name, FunctionSystem* prev_fc_sys) : FunctionSystem(interpreter, prev_fc_sys)
{
    this->name = name;
    this->parent = NULL;
}

Class::Class(Interpreter* interpreter, std::string name, Class* parent) : FunctionSystem(interpreter, parent)
{
    this->name = name;
    this->parent = parent;
}

Class::~Class()
{

}

void Class::addVariable(Variable v)
{
    this->local_variables.push_back(v);
}

Variable Class::getVariable(std::string name)
{
    for (Variable v : this->local_variables)
    {
        if (v.name == name)
            return v;
    }
    throw std::logic_error("No local variable with name: " + name + " has been found");
}

std::vector<Variable> Class::getVariables()
{
    return this->local_variables;
}


bool Class::instanceOf(Class* c)
{
    if (c == NULL)
        throw std::logic_error("NULL class provided");
    Class* current = this;
    while(current != NULL && current != c)
    {
        current = current->parent;
    }
    
    if (c == current)
        return true;
    return false;
}
