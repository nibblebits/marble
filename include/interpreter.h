#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <memory>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
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
    void run(const char* code, PosInfo posInfo);
    void runScript(const char* filename);
    void setCurrentFunctionSystem(FunctionSystem* current_fc_system);
    Logger* getLogger();
    Variable* getVariableByName(std::string name);
    FunctionSystem* getRootFunctionSystem();
    FunctionSystem* getFunctionSystem();
    ClassSystem* getClassSystem();
    Scope* getCurrentScope();
    Scope* getRootScope();
    void setCurrentScope(Scope* scope);
    int getVariableTypeForString(std::string str);
    void interpret_body_node(Node* node);
    void interpret_body(BodyNode* node);
private:
    void new_parented_scope();
    void finish_parented_scope();
    void handleLineAndColumn(PosInfo* posInfo, const char* data, int length);
    void interpret_variable_node(VarNode* var_node);
    void fail();
    // Root systems
    FunctionSystem functionSystem;
    ClassSystem classSystem;
    FunctionSystem* currentFunctionSystem;
    Scope root_scope;
    Scope* current_scope;
    OUTPUT_FUNCTION output;
    Logger logger;
    const char* filename;
};

#endif
