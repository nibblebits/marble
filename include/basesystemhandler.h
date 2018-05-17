#ifndef BASESYSTEMHANDLER_H
#define BASESYSTEMHANDLER_H

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "systemhandler.h"

/**
 * 
 * This is the base SystemHandler class and it contains all base classes, functions and methods that are standard to Marble.
 * An instance of this class should be created and its class system and function system passed to the Interpreter or ModuleSystem.
 * To change the behaviour of this class override it and pass that instead.
 * 
 * Exception and Object are created in this class.
 * 
 * \attention This SystemHandler is as high as the class and function hierarchy can go.
 */ 
class BaseSystemHandler : public SystemHandler
{
public:
    BaseSystemHandler();
    virtual ~BaseSystemHandler();

private:

};

#endif