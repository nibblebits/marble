#include "csystem.h"
#include "systemhandler.h"
#include "variable.h"
#include <iostream>
ClassSystem::ClassSystem()
{
    this->sys_handler = NULL;
    this->defaultBaseClass = NULL;
}

ClassSystem::~ClassSystem()
{

}

void ClassSystem::setSystemHandler(SystemHandler* sys_handler)
{
    if (this->sys_handler != NULL)
        throw std::logic_error("SystemHandler already set");
   
    this->sys_handler = sys_handler;
}

void ClassSystem::setDefaultBaseClass(Class* c)
{
    this->defaultBaseClass = c;
}

Class* ClassSystem::registerClass(std::string class_name, Class* parent)
{
    if (hasClassWithName(class_name))
        throw std::logic_error("The class: " + class_name + " has already been registered");
    
    Class* c; 
    if (parent == NULL && this->defaultBaseClass == NULL)
    {
        c = new Class(this->sys_handler, class_name, this->sys_handler->getRootFunctionSystem());
    }
    else
    {
        if (parent == NULL)
            parent = this->defaultBaseClass;      
        c = new Class(this->sys_handler, class_name, parent);
    }
    this->classes.push_back(std::unique_ptr<Class>(c));
    
    return c;
}

Class* ClassSystem::getClassByName(std::string name)
{
    for (int i = 0; i < this->classes.size(); i++)
    {
        Class* c = this->classes.at(i).get();
        if (c->name == name)
            return c;
    }
   
    return NULL;
}

bool ClassSystem::hasClassWithName(std::string name)
{
    return getClassByName(name) != NULL;
}
