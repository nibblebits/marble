#include "csystem.h"
#include "interpreter.h"
ClassSystem::ClassSystem()
{
    this->interpreter = NULL;
}

ClassSystem::~ClassSystem()
{

}

void ClassSystem::setInterpreter(Interpreter* interpreter)
{
    this->interpreter = interpreter;
}

Class* ClassSystem::registerClass(std::string class_name)
{
    Class* c = new Class(class_name, this->interpreter->getRootFunctionSystem());
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
    
