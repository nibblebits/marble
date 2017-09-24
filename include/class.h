#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <vector>
#include "variable.h"
#include "functionsystem.h"
class Class : public FunctionSystem
{
public:
    Class(std::string name, FunctionSystem* interpreter);
    Class(std::string name, Class* parent);
    virtual ~Class();
    
    void addLocalVariable(Variable v);
    Variable getLocalVariable(std::string name);
    Variable getVariable(std::string name);
    inline std::vector<Variable> getLocalVariables();
    std::vector<Variable> getVariables();
    
    std::string name;
    Class* parent;
private:
    std::vector<Variable> local_variables;
};
#endif
