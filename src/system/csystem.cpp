#include "csystem.h"
#include "systemhandler.h"
#include "variable.h"
#include "object.h"
#include <iostream>
ClassSystem::ClassSystem()
{
    this->sys_handler = NULL;
    this->defaultBaseClass = NULL;
    this->defaultObjectDescriptor = NULL;
    this->prev_sys = NULL;
}

ClassSystem::~ClassSystem()
{

}

void ClassSystem::setDefaultObjectDescriptor(std::shared_ptr<Object> defaultObjectDescriptor)
{
    this->defaultObjectDescriptor = defaultObjectDescriptor;
    
    // We should set the descriptor object that was just set for all registered classes that dont have one.
    for (Class* c : getAllClasses())
    {
        if (c->getDescriptorObject() == NULL)
            c->setDescriptorObject(defaultObjectDescriptor);
    }
}

std::shared_ptr<Object> ClassSystem::getDefaultObjectDescriptor()
{
    return this->defaultObjectDescriptor;
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

Class* ClassSystem::registerClass(std::string class_name, Class* parent, CLASS_REGISTER_RULES rules)
{
    if (!this->defaultObjectDescriptor && !(rules & CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER))
    {
        throw std::logic_error("You must set a default object descriptor before registering a class. Either override with the rule CLASS_REGISTER_OBJECT_DESCRIPTOR_LATER or set a default object descriptor with the \"setDefaultObjectDescriptor\" method");
    }
    
    // If a parent is provided then the object descriptor should equal the parents object descriptor.
    std::shared_ptr<Object> descriptorObj = this->defaultObjectDescriptor;
    if (parent != NULL)
    {
        descriptorObj = parent->getDescriptorObject();
    }
    
    if (hasClassWithName(class_name))
        throw std::logic_error("The class: " + class_name + " has already been registered");
    
    Class* c; 
    if (parent == NULL && this->defaultBaseClass == NULL)
    {
        c = new Class(this->sys_handler, class_name, this->sys_handler->getGlobalFunctionSystem(), descriptorObj);
    }
    else
    {
        if (parent == NULL)
            parent = this->defaultBaseClass;      
        c = new Class(this->sys_handler, class_name, parent, descriptorObj);
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

std::vector<Class*> ClassSystem::getAllClasses()
{
    std::vector<Class*> all_classes;
    if (this->prev_sys != NULL)
    {
        all_classes = this->prev_sys->getAllClasses();
    }
    
    for (int i = 0; i < this->classes.size(); i++)
    {
        Class* c = this->classes.at(i).get();
        all_classes.push_back(c);
    }
    
    return all_classes;
}
