#include "module.h"
Module::Module(std::string name, MODULE_TYPE type)
{
    this->name = name;
    this->type = type;
}

Module::~Module()
{

}

std::string Module::getName()
{
    return this->name;
}

MODULE_TYPE Module::getType()
{
    return this->type;
}