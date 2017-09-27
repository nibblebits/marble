#include "csystem.h"
#include "interpreter.h"
#include <iostream>
ClassSystem::ClassSystem()
{
    this->interpreter = NULL;
    this->defaultBaseClass = NULL;
}

ClassSystem::~ClassSystem()
{

}

void ClassSystem::setInterpreter(Interpreter* interpreter)
{
    if (this->interpreter != NULL)
        throw std::logic_error("Interpreter already set");
   
    this->interpreter = interpreter;
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
        c = new Class(interpreter, class_name, this->interpreter->getRootFunctionSystem());
    }
    else
    {
        if (parent == NULL)
            parent = this->defaultBaseClass;      
        c = new Class(interpreter, class_name, parent);
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
