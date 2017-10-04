#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <vector>
#include <string>
#include "variable.h"
class Object;
class Scope
{
public:
    Scope();
    virtual ~Scope();
    virtual void registerVariable(Variable* variable);
    Variable* createVariable();
    Variable* cloneCreate(Variable* variable);
    Variable* getVariable(std::string variable_name);
    Variable* getVariableAnyScope(std::string variable_name);
    
    // Returns weather this scope is nested in the scope provided.
    bool isNestedInScope(Scope* scope);
    std::vector<Variable*> getVariables();
    std::vector<Variable*> getObjectVariablesFor(std::shared_ptr<Object> object);
    Scope* prev;
private:
    std::vector<Variable*> variables;
    // Holds unique pointers for anyone that called createVariable. Memory will be freed when the scope object leaves its scope
    std::vector<std::unique_ptr<Variable>> unique_variables;
};
#endif
