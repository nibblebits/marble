#include "object.h"
#include "variable.h"
#include "interpreter.h"
#include <iostream>
Object::Object(Interpreter* interpreter, Class* c)
{
    this->interpreter = interpreter;
    if (c == NULL)
        throw std::logic_error("Expecting a non NULL class");
    this->c = c;
    // Let's create variables for the object based by the class variables
    Class* current = c;
    while(current != NULL)
    {
        for (Variable v : current->getVariables())
        {
            cloneCreate(&v);
        }
        current = current->parent;
    }
    prev = interpreter->getRootScope();
}

Object::~Object()
{
 
}

void Object::setup()
{
    // Let's create the "super" and "this" variables
    Variable* variable = createVariable();
    variable->type = VARIABLE_TYPE_OBJECT;
    variable->name = "super";
    variable->value.ovalue = shared_from_this();
    variable->value.holder = variable;

    variable = cloneCreate(variable);
    variable->name = "this";   
}

void Object::registerVariable(Variable* variable)
{
    if (variable == NULL)
        throw std::logic_error("The variable must not be NULL");
        
    variable->object = this;
    Scope::registerVariable(variable);
}

Class* Object::getClass()
{
    return this->c;
}

