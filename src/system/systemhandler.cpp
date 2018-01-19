#include "systemhandler.h"


SystemHandler::SystemHandler(SYSTEM_HANDLER_TYPE type, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem)
{
    this->was_activated = false;
    this->activated = false;
    this->type = type;
    this->current_obj = NULL;
    this->passedBaseClassSystem = baseClassSystem;
    this->creatorsOldBaseClassPrevFunctionSystem = NULL;
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

    if (this->passedBaseClassSystem != NULL) {
        this->classSystem.setDefaultBaseClass(this->passedBaseClassSystem->getDefaultBaseClass());
    }
    this->globalFunctionSystem.setSystemHandler(this);
}

SystemHandler::~SystemHandler()
{
    if (!this->was_activated)
    {
        throw std::logic_error("The SystemHandler provided was never activated, We don't know if this was intential but we thought you should know. Call activate() next time");
    }
    
    if (this->isActive())
    {
        throw std::logic_error("The SystemHandler was never deactivated. Always deactivate the SystemHandler when you are done by calling deactivate()");
    }

    if (!this->current_obj_stack.empty())
    {
        throw std::logic_error("A current object was set but never finished. please call finishCurrentObject when you are done with the object set with setCurrentObject");
    }

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

bool SystemHandler::isActive()
{
    return this->activated;
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

void SystemHandler::activate()
{
    if (this->isActive())
        throw std::logic_error("The SystemHandler is already active");
    
    this->activated = true;
    this->was_activated = true;
    
    if (this->passedBaseClassSystem == NULL)
        return;
    
    if (this->passedBaseClassSystem->getDefaultBaseClass() == NULL)
        return;
    
     this->creatorsOldBaseClassPrevFunctionSystem = this->passedBaseClassSystem->getDefaultBaseClass()->getPreviousFunctionSystem();
            
    /* We must set the previous class system of the default base class that was sent to us to our current function system so that 
     * Functions can be found correctly */
    this->passedBaseClassSystem->getDefaultBaseClass()->setPreviousFunctionSystem(this->currentFunctionSystem);
}

void SystemHandler::deactivate()
{
    if (!this->isActive())
        throw std::logic_error("The SystemHandler was never activated");
    this->activated = false;
    
    if (this->passedBaseClassSystem == NULL)
        return;
        
    if (this->passedBaseClassSystem->getDefaultBaseClass() == NULL)
        return;
    // Now restore the creators old base class previous function system
    this->passedBaseClassSystem->getDefaultBaseClass()->setPreviousFunctionSystem(this->creatorsOldBaseClassPrevFunctionSystem);
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
    this->current_obj_stack.push_back(getCurrentObject());
    this->current_obj = object;
}

void SystemHandler::finishCurrentObject()
{
    this->current_obj = this->current_obj_stack.back();
    this->current_obj_stack.pop_back();
}