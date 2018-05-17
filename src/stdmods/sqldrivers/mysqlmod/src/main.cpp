#include <string>
#include <iostream>
#include "module.h"
#include "../include/mysqlmod.h"
#include "main.h"
Module* marble_mod_init()
{
  return new MysqlModule();
}
