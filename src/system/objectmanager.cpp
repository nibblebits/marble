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

bool ObjectManager::isSafeToDelete(Object* object, Scope* ignored_scope)
{
    Scope* current_scope = this->interpreter->getCurrentScope();
    while(current_scope != NULL)
    {
        if (current_scope != ignored_scope)
        {
            if (!current_scope->getObjectVariablesFor(object).empty())
            {
                return false;
            }
        }
        
        current_scope = current_scope->prev;
    }
    
    return true;
}

void ObjectManager::clean(Scope* scope)
{
    std::vector<Variable*> scope_variables = scope->getVariables();
    for(Variable* variable : scope_variables)
    {
        Value value = variable->value;
        if (value.type == VALUE_TYPE_OBJECT 
                || value.type == VALUE_TYPE_ARRAY)
        {
            Object* obj = NULL;
            if (value.type == VALUE_TYPE_ARRAY)
            {
                obj = value.avalue;
            }
            else
            {
                throw std::logic_error("Variable values not yet supported");
            }
            
            if (isSafeToDelete(obj, scope))
            {
                // Ok lets delete this variable
                std::cout << "Deleted variable in object manager!" << std::endl;
                delete obj;
                objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
            }
        }        
    }
}
