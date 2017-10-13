#ifndef CLASS_H
#define CLASS_H
#include <string>
#include <vector>
#include "variable.h"
#include "functionsystem.h"
class Class : public FunctionSystem
{
public:
    Class(Interpreter* interpreter, std::string name, FunctionSystem* prev_fc_sys);
    Class(Interpreter* interpreter, std::string name, Class* parent);
    virtual ~Class();
    
    void addVariable(Variable v);
    Variable getVariable(std::string name);
    std::vector<Variable> getVariables();
    
    bool instanceOf(Class* c);
    std::string name;
    Class* parent;
private:
    std::vector<Variable> local_variables;
};
#endif
