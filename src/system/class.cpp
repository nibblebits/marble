#include "class.h"
Class::Class(std::string name, FunctionSystem* prev_fc_sys) : FunctionSystem(prev_fc_sys)
{
    this->name = name;
    this->parent = NULL;
}

Class::Class(std::string name, Class* parent) : FunctionSystem(parent)
{
    this->name = name;
    this->parent = parent;
}

Class::~Class()
{

}

