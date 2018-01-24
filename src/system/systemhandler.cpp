#include "systemhandler.h"
#include "scope.h"


SystemHandler::SystemHandler(SYSTEM_HANDLER_TYPE type, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem)
{
    this->type = type;
    this->current_obj.object = NULL;
    this->current_obj.c = NULL;
    this->current_obj.accessors_scope = NULL;
    this->baseClassSystem = baseClassSystem;
    this->currentFunctionSystem = &this->globalFunctionSystem;

    this->classSystem.setSystemHandler(this);
    this->classSystem.setPreviousClassSystem(baseClassSystem);

    /*
    * If a base class system is provided then we must set the default base class of our current class system to that of the base class system.
    * Also so that classes created with this system handler can access functions in our current system handler we must also set the bases base class's previous function system
    * to our own. This will also mean that this base class system provided to us will lose its previous base class's function system which is important to note.
    */
    if (this->baseClassSystem != NULL) {
        Class* base_base_class = this->baseClassSystem->getDefaultBaseClass();
        this->classSystem.setDefaultBaseClass(base_base_class);
        base_base_class->setPreviousFunctionSystem(this->currentFunctionSystem);
    }

    if (baseFunctionSystem == NULL)
        this->baseFunctionSystem = &this->globalFunctionSystem;
    else
        this->baseFunctionSystem = baseFunctionSystem;

    if (baseClassSystem == NULL)
        this->baseClassSystem = &this->classSystem;
    else
        this->baseClassSystem = baseClassSystem;

    this->currentFunctionSystem->setPreviousFunctionSystem(baseFunctionSystem);
    this->globalFunctionSystem.setSystemHandler(this);
   

}

SystemHandler::~SystemHandler()
{

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
    return this->current_obj.object;
}

Class* SystemHandler::getCurrentObjectClass()
{
    return this->current_obj.c;
}

void SystemHandler::setCurrentObject(std::shared_ptr<Object> object, Class* c, Scope* accessors_scope)
{
    this->current_obj_stack.push_back(this->current_obj);

    struct current_object obj;
    obj.object = object;
    obj.c = c;
    obj.accessors_scope = accessors_scope;
    this->current_obj = obj;
}

void SystemHandler::finishCurrentObject()
{
    this->current_obj = this->current_obj_stack.back();
    this->current_obj_stack.pop_back();
}

Scope* SystemHandler::getAccessorsScope()
{
    return this->current_obj.accessors_scope;
}

bool SystemHandler::isAccessingObject()
{
    return getCurrentObject() != NULL;
}