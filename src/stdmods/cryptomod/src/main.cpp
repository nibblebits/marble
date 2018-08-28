#include <string>
#include <iostream>
#include "module.h"
#include "../include/cryptomod.h"
#include "main.h"
Module* marble_mod_init()
{
  return new CryptoModule();
}
