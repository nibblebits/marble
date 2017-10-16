#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string.h>
#include <memory>
#include <vector>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
#include "systemhandler.h"
#include "logger.h"

typedef std::function<void(const char* output)> OUTPUT_FUNCTION;

class Node;
class VarNode;
class ExpNode;
class BodyNode;
class Object;
class Interpreter : public SystemHandler
{
public:
    Interpreter(ClassSystem* classSystem, FunctionSystem* baseFunctionSystem);
    virtual ~Interpreter();
    void setOutputFunction(OUTPUT_FUNCTION output);
    void ready();
    void run(const char* code, PosInfo posInfo);
    void runScript(const char* filename);

private:
    void handleLineAndColumn(PosInfo* posInfo, const char* data, int length);
    void fail();
    const char* filename;
    OUTPUT_FUNCTION output;
};

#endif
