#include <string>
#include <iostream>
#include "main.h"
#include "iomod.h"
Module* marble_mod_init(Interpreter* interpreter)
{
    return new IOModule();
}

void test()
{

}