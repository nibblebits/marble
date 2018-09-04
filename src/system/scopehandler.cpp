#include "scopehandler.h"
#include "variable.h"
#include <iostream>

ScopeHandler::ScopeHandler()
{
    // Create a root scope with NULL permissions.
    this->root_scope = std::unique_ptr<Scope>(new Scope(NULL));
    this->action_scope = root_scope.get();
    this->current_scope = root_scope.get();

    // We don't want the root scope to be our current scope as root scope is for native variables
    new_parented_scope();
    this->parented_scope = getCurrentScope();
    this->action_scope = getCurrentScope();
    this->global_scope = getCurrentScope();
}

ScopeHandler::~ScopeHandler()
{
    /* We can't gaurantee that the ScopeHandler is pointing at the parented scope we created on construction 
    this is why we will delete it manually instead of calling finish_parented_scope() */
    delete this->parented_scope;
}

Scope *ScopeHandler::getCurrentScope()
{
    return this->current_scope;
}

Scope *ScopeHandler::getRootScope()
{
    return this->root_scope.get();
}

Scope *ScopeHandler::getGlobalScope()
{
    return this->global_scope;
}

void ScopeHandler::setCurrentScope(Scope *scope)
{
    if (scope == NULL)
        throw std::logic_error("Was not expecting a NULL scope");

    if (this->current_scope != NULL)
        this->current_scope->onLeaveScope(true);
    this->current_scope = scope;
    this->current_scope->onEnterScope(true);
}

void ScopeHandler::new_parented_scope()
{
    Scope *new_prev = current_scope;
    // New scopes must inherit parent scopes permissions.
    current_scope = new Scope(this->current_scope->permissions);
    current_scope->onEnterScope(false);
    current_scope->prev = new_prev;
}

void ScopeHandler::finish_parented_scope()
{
    Scope *old_current = current_scope;
    old_current->onLeaveScope(false);
    current_scope = old_current->prev;
    delete old_current;
}

Variable *ScopeHandler::getVariableByName(std::string name)
{
    Variable *variable = getCurrentScope()->getVariableAnyScope(name);
    if (variable == NULL)
    {
        throw std::logic_error("Variable not found: " + name);
    }

    return variable;
}

void ScopeHandler::useScope(std::function<void()> function, Scope *scope)
{
    if (scope == NULL)
        scope = getCurrentScope();

    Scope *old_scope = getCurrentScope();
    setCurrentScope(scope);
    try
    {
        function();
    }
    catch (...)
    {
        setCurrentScope(old_scope);
        throw;
    }
    setCurrentScope(old_scope);
}