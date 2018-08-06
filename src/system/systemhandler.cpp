#include "systemhandler.h"
#include "scope.h"
#include <iostream>

SystemHandler::SystemHandler(SYSTEM_HANDLER_TYPE type, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem, SYSTEM_HANDLER_RULES rules)
{
    this->type = type;
    this->current_obj.object = NULL;
    this->current_obj.c = NULL;
    this->baseClassSystem = baseClassSystem;
    this->currentFunctionSystem = &this->globalFunctionSystem;

    this->classSystem.setSystemHandler(this);
    this->classSystem.setPreviousClassSystem(baseClassSystem);

    /*
    * If a base class system is provided then we must set the default base class of our current class system to that of the base class system.
    * Also so that classes created with this system handler can access functions in our current system handler we must also set the bases base class's previous function system
    * to our own. Note that the previous function system will only be set on the base class if it already doesn't have a previous function system.
    */
    if (this->baseClassSystem != NULL && !(rules & SYSTEM_HANDLER_NO_PARENT_BASE_CLASS_LINK)) {
        Class* base_base_class = this->baseClassSystem->getDefaultBaseClass();
        this->classSystem.setDefaultBaseClass(base_base_class);
        if (base_base_class->getPreviousFunctionSystem() == NULL)
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

void SystemHandler::useFunctionSystem(FunctionSystem* f_system, std::function<void()> call)
{
    FunctionSystem* old_fc_sys = getFunctionSystem();
    this->setFunctionSystem(f_system);
    try
    {
        call();
    }
    catch(...)
    {
        this->setFunctionSystem(old_fc_sys);
        throw;
    }
    this->setFunctionSystem(old_fc_sys);

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

void SystemHandler::setCurrentObject(std::shared_ptr<Object> object, Class* c)
{
    this->current_obj_stack.push_back(this->current_obj);

    struct current_object obj;
    obj.object = object;
    obj.c = c;
    this->current_obj = obj;
}

void SystemHandler::finishCurrentObject()
{
    this->current_obj = this->current_obj_stack.back();
    this->current_obj_stack.pop_back();
}

bool SystemHandler::isAccessingObject()
{
    return getCurrentObject() != NULL;
}

void SystemHandler::save()
{
    this->rules_stack.push_back(this->rules);
    struct rules r;
    this->rules = r;
}

void SystemHandler::restore()
{
    this->rules = this->rules_stack.back();
    this->rules_stack.pop_back();
}


void SystemHandler::expectingArray(int dimensions)
{
    if (!isExpecting())
        throw std::logic_error("You must start expecting a type before specifying the array dimensions");

    this->rules.expected_array_dimensions = dimensions;
}

void SystemHandler::expecting(std::string type)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");
    this->rules.expecting_type = type;
    this->rules.expecting_value_type = Value::getValueTypeForString(type);
    this->rules.expecting_variable_type = Variable::getVariableTypeForString(type);
}

bool SystemHandler::isExpecting()
{
    return this->rules.expecting_type != "";
}

bool SystemHandler::isExpectingArray()
{
    return this->rules.expected_array_dimensions > 0;
}

int SystemHandler::getExpectedArrayDimensions()
{
    return this->rules.expected_array_dimensions;
}

void SystemHandler::endExpecting()
{
    this->rules.expecting_type = "";
    this->rules.expected_array_dimensions = 0;
}

std::string SystemHandler::getExpectingType()
{
    return this->rules.expecting_type;
}

VALUE_TYPE SystemHandler::getExpectingValueType()
{
    return this->rules.expecting_value_type;
}

VARIABLE_TYPE SystemHandler::getExpectingVariableType()
{
    return this->rules.expecting_variable_type;
}


