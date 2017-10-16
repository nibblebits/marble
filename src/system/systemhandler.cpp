#include "systemhandler.h"


SystemHandler::SystemHandler(SYSTEM_HANDLER_TYPE type, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem)
{
    this->type = type;
    this->current_obj = NULL;
    if (baseFunctionSystem == NULL)
        this->baseFunctionSystem = &this->globalFunctionSystem;
    else
        this->baseFunctionSystem = baseFunctionSystem;
    if (baseClassSystem == NULL)
        this->baseClassSystem = &this->classSystem;
    else
        this->baseClassSystem = baseClassSystem;
        
    this->currentFunctionSystem = &this->globalFunctionSystem;
    this->currentFunctionSystem->setPreviousFunctionSystem(baseFunctionSystem);
    this->classSystem.setPreviousClassSystem(baseClassSystem);
    this->classSystem.setSystemHandler(this);
    if (baseClassSystem != NULL)
        this->classSystem.setDefaultBaseClass(baseClassSystem->getDefaultBaseClass());
    this->globalFunctionSystem.setSystemHandler(this);
}

SystemHandler::~SystemHandler()
{

}


FunctionSystem* SystemHandler::getGlobalFunctionSystem()
{
    return &this->globalFunctionSystem;
}

FunctionSystem* SystemHandler::getFunctionSystem()
{
    return this->currentFunctionSystem;
}

FunctionSystem* SystemHandler::getBaseFunctionSystem()
{
    return this->baseFunctionSystem;
}

ClassSystem* SystemHandler::getClassSystem()
{
    return &this->classSystem;
}

ClassSystem* SystemHandler::getBaseClassSystem()
{
    return this->baseClassSystem;
}


void SystemHandler::setFunctionSystem(FunctionSystem* current_fc_system)
{
    this->currentFunctionSystem = current_fc_system;
}

Logger* SystemHandler::getLogger()
{
    return &this->logger;
}

SYSTEM_HANDLER_TYPE SystemHandler::getType()
{
    return this->type;
}

std::shared_ptr<Object> SystemHandler::getCurrentObject()
{
    return this->current_obj;
}

void SystemHandler::setCurrentObject(std::shared_ptr<Object> object)
{
    this->current_obj = object;
}

