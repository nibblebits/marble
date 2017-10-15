#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <vector>
#include "variable.h"
#include "functionsystem.h"
class Class : public FunctionSystem
{
public:
    Class(SystemHandler* sys_handler, std::string name, FunctionSystem* prev_fc_sys);
    Class(SystemHandler* sys_handler, std::string name, Class* parent);
    virtual ~Class();
    
    void addVariable(Variable v);
    bool hasVariableWithName(std::string name);
    Variable getVariable(std::string name);
    std::vector<Variable> getVariables();
    
    Class* getClassWhoHasVariable(std::string name);
    
    bool instanceOf(Class* c);
    std::string name;
    Class* parent;
private:
    std::vector<Variable> local_variables;
};
#endif
