#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include "scope.h"
#include "functionsystem.h"
class Node;
class VarNode;
class ExpNode;
class Interpreter
{
public:
    Interpreter();
    virtual ~Interpreter();
    void output(const char* data);
    void run(const char* code);
    void runScript(const char* filename);
    Variable* getVariableByName(std::string name);
    FunctionSystem* getFunctionSystem();
private:
    void interpret_variable_node_for_primitive(VarNode* var_node);
    void interpret_variable_node(VarNode* var_node);
    void fail();
    double op_on_values(double value1, double value2, std::string op);
    FunctionSystem functionSystem;
    Scope root_scope;
    Scope* current_scope;

};

#endif
