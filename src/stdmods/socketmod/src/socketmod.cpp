#include "socketmod.h"
#include "object.h"
#include "exceptions/systemexception.h"
#include "exceptionobject.h"
#include "function.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

SocketModule::SocketModule() : Module("socketmod", "Socket Module", MODULE_TYPE_MARBLE_LIBRARY)
{

}

SocketModule::~SocketModule()
{

}

void SocketModule::Init()
{
  
}

void SocketModule::newInterpreter(Interpreter* interpreter)
{
  
}