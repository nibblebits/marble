#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <memory>
#include <vector>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
#include "scopehandler.h"
#include "logger.h"

typedef std::function<void(const char* output)> OUTPUT_FUNCTION;

class Node;
class VarNode;
class ExpNode;
class BodyNode;
class Object;
class Interpreter : public ScopeHandler
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
private:
    void handleLineAndColumn(PosInfo* posInfo, const char* data, int length);
    void fail();
    // Root systems
    FunctionSystem functionSystem;
    ClassSystem classSystem;
    FunctionSystem* currentFunctionSystem;
    OUTPUT_FUNCTION output;
    Logger logger;
    const char* filename;
};

#endif
