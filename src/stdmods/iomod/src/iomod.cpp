#include "iomod.h"
#include <iostream>
IOModule::IOModule() : Module("IO Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

IOModule::~IOModule()
{

}

void IOModule::Init()
{
    std::cout << "IOModule INIT SUCCESS" << std::endl;
}