#ifndef SYSTEMHANDLER_H
#define SYSTEMHANDLER_H
#include <string.h>
#include <memory>
#include <vector>
#include "scope.h"
#include "functionsystem.h"
#include "csystem.h"
#include "scopehandler.h"
#include "logger.h"

class SystemHandler : public ScopeHandler
{
public:
    SystemHandler();
    virtual ~SystemHandler();
    void setFunctionSystem(FunctionSystem* current_fc_system);
    Logger* getLogger();
    FunctionSystem* getRootFunctionSystem();
    FunctionSystem* getFunctionSystem();
    ClassSystem* getClassSystem();
protected:
    // Root systems
    FunctionSystem functionSystem;
    ClassSystem classSystem;
    FunctionSystem* currentFunctionSystem;
    Logger logger;
};
#endif
