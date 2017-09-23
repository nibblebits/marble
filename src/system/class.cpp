#include "class.h"
Class::Class(std::string name, FunctionSystem* prev_fc_sys) : FunctionSystem()
{
    this->name = name;
    this->prev_fc_sys = prev_fc_sys;
}
Class::~Class()
{

}

