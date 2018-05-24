#include "class.h"
#include "function.h"
#include "interpreter.h"
#include "fnode.h"
Class::Class(SystemHandler* sys_handler, std::string name, FunctionSystem* prev_fc_sys, std::shared_ptr<Object> descriptor_obj) : FunctionSystem(sys_handler, prev_fc_sys)
{
    this->name = name;
    this->parent = NULL;
    this->descriptor_obj = descriptor_obj;
    this->is_pure = false;
    this->is_final = false;
}

Class::Class(SystemHandler* sys_handler, std::string name, Class* parent, std::shared_ptr<Object> descriptor_obj) : FunctionSystem(sys_handler, parent)
{
    this->name = name;
    this->parent = parent;
    this->descriptor_obj = descriptor_obj;
    this->is_pure = false;
    this->is_final = false;
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

void Class::setDescriptorObject(std::shared_ptr<Object> object)
{
    this->descriptor_obj = object;
}

std::shared_ptr<Object> Class::getDescriptorObject()
{
    return this->descriptor_obj;
}

void Class::invokeObjectParentConstructor(std::vector<Value> values, std::shared_ptr<Object> object, Interpreter* interpreter)
{
    // We don't care about the return value as this is a constructor
    Value return_value;

    Function* parent_constructor = this->parent->getFunctionByName("__construct");
    // The caller scope will be the scope before invoking the constructor
    Scope* caller_scope = interpreter->getCurrentScope();
    if (parent_constructor != NULL)
    {
        parent_constructor->invoke(interpreter, values, &return_value, object, caller_scope);
    }    
}

Function* Class::registerFunction(std::string name, std::vector<VarType> args, VarType return_type, NATIVE_FUNCTION_ENTRYPOINT entrypoint)
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

bool Class::instanceOf(std::string class_name)
{
    Class* current = this;
    while(current != NULL && current->name != class_name)
    {
        current = current->parent;
    }
    
    if (current != NULL && class_name == current->name)
        return true;
    return false;
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

