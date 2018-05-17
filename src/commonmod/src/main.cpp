#include <string>
#include <iostream>
#include "module.h"
#include "../include/commonmod.h"
#include "../include/commonmod_main.h"
Module* marble_mod_init()
{
  return new CommonModule();
}
