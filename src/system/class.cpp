#include "class.h"
#include "function.h"
Class::Class(SystemHandler* sys_handler, std::string name, FunctionSystem* prev_fc_sys) : FunctionSystem(sys_handler, prev_fc_sys)
{
    this->name = name;
    this->parent = NULL;
}

Class::Class(SystemHandler* sys_handler, std::string name, Class* parent) : FunctionSystem(sys_handler, parent)
{
    this->name = name;
    this->parent = parent;
}

Class::~Class()
{

}

Class* Class::getClassWhoHasVariable(std::string name)
{
    if (hasVariableWithName(name))
        return this;
    
    if (this->parent != NULL)
        return this->parent->getClassWhoHasVariable(name); 
    
    return NULL;
}

Function* Class::registerFunction(std::string name, std::vector<VarType> args, VarType return_type, std::function<void(std::vector<Value> values, Value* return_value, std::shared_ptr<Object> object)> entrypoint)
{
    Function* function = FunctionSystem::registerFunction(name, args, return_type, entrypoint);
    function->cls = this;
    return function;
}

Function* Class::registerFunction(FunctionNode* fnode)
{
    Function* function = FunctionSystem::registerFunction(fnode);
    function->cls = this;
    return function;
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

bool Class::hasVariableWithName(std::string name)
{
    for (Variable v : this->local_variables)
    {
        if (v.name == name)
            return true;
    } 
    
    return false;
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

