#include "systemhandler.h"

SystemHandler::SystemHandler()
{

}

SystemHandler::~SystemHandler()
{

}


FunctionSystem* SystemHandler::getRootFunctionSystem()
{
    return &this->functionSystem;
}

FunctionSystem* SystemHandler::getFunctionSystem()
{
    return this->currentFunctionSystem;
}

ClassSystem* SystemHandler::getClassSystem()
{
    return &this->classSystem;
}


void SystemHandler::setFunctionSystem(FunctionSystem* current_fc_system)
{
    this->currentFunctionSystem = current_fc_system;
}

Logger* SystemHandler::getLogger()
{
    return &this->logger;
}

