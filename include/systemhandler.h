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
    SystemHandler(SYSTEM_HANDLER_TYPE type);
    virtual ~SystemHandler();
    void setFunctionSystem(FunctionSystem* current_fc_system);
    Logger* getLogger();

    std::shared_ptr<Object> getCurrentObject();
    void setCurrentObject(std::shared_ptr<Object> object);
    FunctionSystem* getRootFunctionSystem();
    FunctionSystem* getFunctionSystem();
    ClassSystem* getClassSystem();
    SYSTEM_HANDLER_TYPE getType();
protected:
    // Root systems
    FunctionSystem functionSystem;
    ClassSystem classSystem;
    FunctionSystem* currentFunctionSystem;
    Logger logger;
    SYSTEM_HANDLER_TYPE type;
    std::shared_ptr<Object> current_obj;
};
#endif
