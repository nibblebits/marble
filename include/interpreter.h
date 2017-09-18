#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <memory>
#include "scope.h"
#include "functionsystem.h"
#include "logger.h"

typedef std::function<void(const char* output)> OUTPUT_FUNCTION;

class Node;
class VarNode;
class ExpNode;
class BodyNode;
class Interpreter
{
public:
    Interpreter();
    virtual ~Interpreter();
    void setOutputFunction(OUTPUT_FUNCTION output);
    void ready();
    void run(const char* code);
    void runScript(const char* filename);
    Logger* getLogger();
    Variable* getVariableByName(std::string name);
    FunctionSystem* getFunctionSystem();
    Scope* getCurrentScope();
    Scope* getRootScope();
    int getVariableTypeForString(std::string str);
    void interpret_body_node(Node* node);
    void interpret_body(BodyNode* node);
private:
    void new_parented_scope();
    void finish_parented_scope();
    void interpret_variable_node_for_primitive(VarNode* var_node);
    void interpret_variable_node(VarNode* var_node);
    void fail();
    double op_on_values(double value1, double value2, std::string op);
    FunctionSystem functionSystem;
    Scope root_scope;
    Scope* current_scope;
    OUTPUT_FUNCTION output;
    Logger logger;
};

#endif
