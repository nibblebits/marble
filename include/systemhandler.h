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
    SystemHandler(SYSTEM_HANDLER_TYPE type, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem);
    virtual ~SystemHandler();
    void setFunctionSystem(FunctionSystem* current_fc_system);
    Logger* getLogger();

    /** Should be called when this SystemHandler is activate, e.g during validation or interpreting */
    void activate();
    /** Should be called when this SystemHandler is to be deactivated, e.g after validating or interpreting */
    void deactivate();
    
    /**
    * Gets the current Object that is currently being accessed. This object will be the value of the object that currently a method is being called on or an attribute being accessed on.. E.g obj.function();
    */
    std::shared_ptr<Object> getCurrentObject();
    /**
    * Used to set the current Object that is currently being accessed. This object must be the value of the object that currently a method is being called on or an attribute being accessed on.. E.g obj.function();
    */
    void setCurrentObject(std::shared_ptr<Object> object);
    FunctionSystem* getGlobalFunctionSystem();
    FunctionSystem* getFunctionSystem();
    FunctionSystem* getBaseFunctionSystem();
    ClassSystem* getBaseClassSystem();
    ClassSystem* getClassSystem();
    
    bool isActive();
    SYSTEM_HANDLER_TYPE getType();
protected:
    // The base class system is where the systems classes from native libraries are registered
    ClassSystem* baseClassSystem;
    ClassSystem classSystem;
    // The base function system is where the systems functions from native libraries are registered
    FunctionSystem* baseFunctionSystem;
    FunctionSystem globalFunctionSystem;
    FunctionSystem* currentFunctionSystem;
    Logger logger;
    SYSTEM_HANDLER_TYPE type;
    std::shared_ptr<Object> current_obj;
private:
    ClassSystem* passedBaseClassSystem;
    FunctionSystem* creatorsOldBaseClassPrevFunctionSystem;
    bool activated;
    bool was_activated;
};
#endif
