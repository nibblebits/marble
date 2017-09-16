#include "objectmanager.h"
#include "interpreter.h"
#include "scope.h"
#include "object.h"
#include "array.h"

#include <algorithm>
#include <iostream>

ObjectManager::ObjectManager(Interpreter* interpreter)
{
    this->interpreter = interpreter;
}

ObjectManager::~ObjectManager()
{
    // We have been destructed lets kill every object
    for (Object* object : this->objects)
    {
        delete object;
    }
}

void ObjectManager::registerObject(Object* object)
{
    this->objects.push_back(object);
}

void ObjectManager::incrementUsage(Object* object)
{
    object->usage++;
}

void ObjectManager::decrementUsage(Object* object)
{
    object->usage--;
}


void ObjectManager::clean()
{
    for (int i = 0; i < this->objects.size(); i++)
    {
        Object* object = this->objects.at(i);
        if (object->usage <= 0)
        {
            delete object;
        }
        this->objects.erase(this->objects.begin()+i);
    }
}

