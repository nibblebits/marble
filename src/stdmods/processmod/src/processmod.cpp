#include "processmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "shellobject.h"
#include "shellpermission.h"
#include "shellexecutionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>

ProcessModule::ProcessModule() :  Module("processmod", "Process Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

ProcessModule::~ProcessModule()
{

}

void ProcessModule::Init()
{

    // Register the ShellPermission class
    ShellPermission::registerClass(this->getModuleSystem());

    // Register the ShellExecutionObject
    ShellExecutionObject::registerClass(this->getModuleSystem());

    // Register the ShellObject
    ShellObject::registerClass(this->getModuleSystem());
}

void ProcessModule::newInterpreter(Interpreter *interpreter)
{

}