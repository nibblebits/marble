#include <string>
#include <iostream>
#include "module.h"
#include "../include/mathmod.h"
#include "main.h"
Module* marble_mod_init()
{
  return new MathModule();
}
