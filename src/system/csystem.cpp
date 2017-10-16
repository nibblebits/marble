#include "csystem.h"
#include "systemhandler.h"
#include "variable.h"
#include <iostream>
ClassSystem::ClassSystem()
{
    this->sys_handler = NULL;
    this->defaultBaseClass = NULL;
    this->prev_sys = NULL;
}

ClassSystem::~ClassSystem()
{

}

void ClassSystem::setPreviousClassSystem(ClassSystem* prev_sys)
{
    this->prev_sys = prev_sys;
}

ClassSystem* ClassSystem::getPreviousClassSystem()
{
    return this->prev_sys;
}

void ClassSystem::setSystemHandler(SystemHandler* sys_handler)
{
    this->sys_handler = sys_handler;
}

void ClassSystem::setDefaultBaseClass(Class* c)
{
    this->defaultBaseClass = c;
}

Class* ClassSystem::getDefaultBaseClass()
{
    return this->defaultBaseClass;
}

Class* ClassSystem::registerClass(std::string class_name, Class* parent)
{
    if (hasClassWithName(class_name))
        throw std::logic_error("The class: " + class_name + " has already been registered");
    
    Class* c; 
    if (parent == NULL && this->defaultBaseClass == NULL)
    {
        c = new Class(this->sys_handler, class_name, this->sys_handler->getGlobalFunctionSystem());
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
   
    if (this->prev_sys != NULL)
        return this->prev_sys->getClassByName(name);
        
    return NULL;
}

bool ClassSystem::hasClassWithName(std::string name)
{
    return getClassByName(name) != NULL;
}
