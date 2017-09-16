#include "object.h"
#include "objectmanager.h"
Object::Object(ObjectManager* objectManager)
{
    this->objectManager = objectManager;
    this->usage = 0;
}

Object::~Object()
{
    
}

void Object::decrementUsage()
{
    this->objectManager->decrementUsage(this);
}

void Object::incrementUsage()
{
    this->objectManager->incrementUsage(this);
}
