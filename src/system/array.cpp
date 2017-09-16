#include "array.h"
#include "variable.h"
#include "objectmanager.h"

Array::Array(ObjectManager* objectManager, Variable* variables, int count) : Object(objectManager)
{
    this->variables = variables;
    this->count = count;
}


Array::~Array()
{

}




