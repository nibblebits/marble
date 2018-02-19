#include <string>
#include <iostream>
#include "module.h"
#include "../include/commonmod.h"
#include "main.h"
Module* marble_mod_init()
{
  return new CommonModule();
}
