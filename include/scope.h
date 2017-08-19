#ifndef SCOPE_H
#define SCOPE_H

#include <vector>
#include <string>
#include "variable.h"
class Scope
{
public:
    Scope();
    virtual ~Scope();
    void registerVariable(Variable* variable);
    Variable* getVariable(std::string variable_name);
    std::vector<Variable*> getVariables();
private:
    std::vector<Variable*> variables;
};
#endif
