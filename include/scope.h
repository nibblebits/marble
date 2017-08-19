#ifndef SCOPE_H
#define SCOPE_H

#include <memory>
#include <vector>
#include <string>
#include "variable.h"
class Scope
{
public:
    Scope();
    virtual ~Scope();
    void registerVariable(Variable* variable);
    Variable* createVariable();
    Variable* getVariable(std::string variable_name);
    std::vector<Variable*> getVariables();
private:
    std::vector<Variable*> variables;
    // Holds unique pointers for anyone that called createVariable. Memory will be freed when the scope object leaves its scope
    std::vector<std::unique_ptr<Variable>> unique_variables;
};
#endif
