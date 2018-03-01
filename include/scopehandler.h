#ifndef SCOPEHANDLER_H
#define SCOPEHANDLER_H
#include "scope.h"
#include <functional>

class Variable;
class ScopeHandler
{
public:
    ScopeHandler();
    virtual ~ScopeHandler();
    Scope* getCurrentScope();
    Scope* getRootScope();
    
    void setCurrentScope(Scope* scope);
    void new_parented_scope();
    void finish_parented_scope();
    Variable* getVariableByName(std::string name);

    /**
     * Sets the current scope to the scope provided and then calls the function provided.
     * After exection of the function is complete the previous scope is restored
     * \param function The function to call
     * \param scope The scope to set as our current scope for when we call the function provided
     */
    void useScope(std::function<void()> function, Scope* scope);
private:
    Scope root_scope;
    Scope* current_scope;
    /*
    * The action_scope holds the scope that any action is preformed on. For example
    * if a variable is set from the root scope such as. foo.bar = 50; Then during the point of foo.bar being set the action scope will be equal
    * to the root scope. */
    Scope* action_scope;
};
#endif
