#include "scopehandler.h"
#include "variable.h"
#include <iostream>
ScopeHandler::ScopeHandler()
{
    this->action_scope = &root_scope;
    this->current_scope = &root_scope;
}

ScopeHandler::~ScopeHandler()
{

}

Scope* ScopeHandler::getCurrentScope()
{
    return this->current_scope;
}

Scope* ScopeHandler::getRootScope()
{
    return &this->root_scope;
}

void ScopeHandler::setCurrentScope(Scope* scope)
{
    if (scope == NULL)
        throw std::logic_error("Was not expecting a NULL scope");
        
    if (this->current_scope != NULL)        
        this->current_scope->onLeaveScope();
    this->current_scope = scope;
    this->current_scope->onEnterScope();
}

void ScopeHandler::new_parented_scope()
{
    Scope* new_prev = current_scope;
    current_scope = new Scope();
    current_scope->onEnterScope();
    current_scope->prev = new_prev;
}

void ScopeHandler::finish_parented_scope()
{
    Scope* old_current = current_scope;
    old_current->onLeaveScope();
    current_scope = old_current->prev;
    delete old_current;
}

Variable* ScopeHandler::getVariableByName(std::string name)
{
    Variable* variable = getCurrentScope()->getVariableAnyScope(name);
    if (variable == NULL)
    {
        for (Variable* var : getCurrentScope()->getVariables())
        {
            std::cout << "VARIABLE IN SCOPE: " << var->name << std::endl;
        }
        throw std::logic_error("Variable not found: " + name);
    }

    return variable;
}

void ScopeHandler::useScope(std::function<void()> function, Scope* scope)
{
    if (scope == NULL)
        scope = getCurrentScope();
        
    Scope* old_scope = getCurrentScope();
    setCurrentScope(scope);
    try
    {
        function();
    } catch(...)
    {
        setCurrentScope(old_scope);
        throw;
    }
    setCurrentScope(old_scope);
}