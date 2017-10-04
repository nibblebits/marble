#include "scopehandler.h"
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
    this->current_scope = scope;
}

Scope* ScopeHandler::getActionScope()
{
    return this->action_scope;
}

void ScopeHandler::new_parented_scope()
{
    this->action_scope = current_scope;
    Scope* new_prev = current_scope;
    current_scope = new Scope();
    current_scope->prev = new_prev;
}

void ScopeHandler::finish_parented_scope()
{
    Scope* old_current = current_scope;
    current_scope = old_current->prev;
    this->action_scope = current_scope;
    delete old_current;
}
