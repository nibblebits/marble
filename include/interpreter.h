#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <memory>
#include <vector>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
#include "logger.h"

typedef std::function<void(const char* output)> OUTPUT_FUNCTION;

class Node;
class VarNode;
class ExpNode;
class BodyNode;
class Object;
class Interpreter
{
public:
    Interpreter();
    virtual ~Interpreter();
    void setOutputFunction(OUTPUT_FUNCTION output);
    void ready();
    void run(const char* code, PosInfo posInfo);
    void runScript(const char* filename);
    void setFunctionSystem(FunctionSystem* current_fc_system);
    Logger* getLogger();
    Variable* getVariableByName(std::string name);
    FunctionSystem* getRootFunctionSystem();
    FunctionSystem* getFunctionSystem();
    ClassSystem* getClassSystem();
    Scope* getCurrentScope();
    Scope* getRootScope();
    void setCurrentScope(Scope* scope);
    Scope* getActionScope();
    void new_parented_scope();
    void finish_parented_scope();
private:
    void handleLineAndColumn(PosInfo* posInfo, const char* data, int length);
    void fail();
    // Root systems
    FunctionSystem functionSystem;
    ClassSystem classSystem;
    FunctionSystem* currentFunctionSystem;
    Scope root_scope;
    Scope* current_scope;
    /*
    * The action_scope holds the scope that any action is preformed on. For example
    * if a variable is set from the root scope such as. foo.bar = 50; Then during the point of foo.bar being set the action scope will be equal
    * to the root scope. */
    Scope* action_scope;
    OUTPUT_FUNCTION output;
    Logger logger;
    const char* filename;
};

#endif
