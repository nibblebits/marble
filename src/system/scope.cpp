#include "scope.h"
#include "object.h"
#include "array.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include "exceptions/testerror.h"
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <iostream>

Scope::Scope(std::shared_ptr<PermissionsObject> permissions)
{
    this->prev = NULL;
    this->last_registered_variable = NULL;
    this->permissions = permissions;
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
    variable->scope = this;
    this->last_registered_variable = variable;
    this->variables.push_back(variable);
}

Variable* Scope::createVariable()
{
    Variable* variable = new Variable();
    this->unique_variables.push_back(std::unique_ptr<Variable>(variable));
    registerVariable(variable);
    return variable;
}


Variable* Scope::createVariable(std::string name, double value, MODIFIER_ACCESS modifier_access)
{
    Variable* variable = this->createVariable();
    variable->value.type = VALUE_TYPE_NUMBER;
    variable->value.dvalue = value;
    variable->value.holder = variable;
    variable->name = name;
    variable->access = modifier_access;
    variable->type = VARIABLE_TYPE_NUMBER;
    variable->type_name = "number";
    return variable;
}

Variable* Scope::createVariable(std::string name, std::string type, std::shared_ptr<Object> value, MODIFIER_ACCESS modifier_access)
{
    bool is_array = std::dynamic_pointer_cast<Array>(value) != NULL;
    if (is_array)
        throw std::logic_error("Array variables currently cannot be created with these quick methods as this functionality is not yet implemented");

    Variable* variable = this->createVariable();
    variable->value.type = VALUE_TYPE_OBJECT;
    variable->value.ovalue = value;
    variable->value.holder = variable;
    variable->name = name;
    variable->access = modifier_access;
    variable->type = VARIABLE_TYPE_OBJECT;
    variable->type_name = type;
    return variable;
}

Variable* Scope::cloneCreate(Variable* variable)
{
    Variable* new_variable = createVariable();
    new_variable->type = variable->type;
    new_variable->type_name = variable->type_name;
    new_variable->name = variable->name;
    new_variable->value = variable->value;
    new_variable->value.holder = new_variable;
    new_variable->scope = this;
    new_variable->access = variable->access;
    return new_variable;
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

/*
* Gets the variable with the name "variable_name" from within this scope or any scope above it
* if it is not found then NULL is returned
*/
Variable* Scope::getVariableAnyScope(std::string variable_name)
{
    Variable* variable = NULL;
    Scope* scope = this;
    while(scope != NULL)
    {
        variable = scope->getVariable(variable_name);
        if (variable != NULL)
        {
            break;
        }
        
        scope = scope->prev;
    }
    
    return variable;
}

Variable* Scope::getLastRegisteredVariable()
{
    return this->last_registered_variable;
}

void Scope::removeVariable(Variable* variable)
{
     variables.erase(std::remove_if(variables.begin(), variables.end(),
                       [&](Variable* v) { return v == variable; }), variables.end());
     unique_variables.erase(std::remove_if(unique_variables.begin(), unique_variables.end(),
                    [&](std::unique_ptr<Variable>& v) { return v.get() == variable; }), unique_variables.end());
}

// Events
void Scope::onEnterScope()
{

}

void Scope::onLeaveScope()
{

}
bool Scope::isNestedInScope(Scope* scope)
{
    if (scope == NULL)
        throw std::logic_error("The scope provided cannot be NULL");
        
    if (scope == this)
        return true;
    
    
    Scope* current = prev;
    while(current != NULL && current != scope)
    {
        current = current->prev;
    }
    
    return current == scope;
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


std::shared_ptr<PermissionObject> Scope::getPermission(std::string name)
{
    if (this->permissions == NULL)
    {
        return NULL;
    }

    std::shared_ptr<PermissionObject> permission = std::dynamic_pointer_cast<PermissionObject>(permissions->getPermission(name));
    return permission;
}