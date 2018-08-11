#include "validator.h"
#include "nodes.h"
#include "logger.h"
#include "object.h"
#include "class.h"
#include "interpreter.h"
#include "csystem.h"
#include <iostream>
#include "exceptions/testerror.h"
Validator::Validator(Logger* logger, Interpreter* interpreter) : SystemHandler(SYSTEM_HANDLER_VALIDATOR, interpreter->getClassSystem(), interpreter->getFunctionSystem())
{
    this->interpreter = interpreter;
    this->logger = logger;
    this->current_class = NULL;
    
    ClassSystem* c_system = interpreter->getClassSystem();
    // We must create class objects for all current classes so that they will be compatible with the validation system
    for (Class* c : c_system->getAllClasses())
    {
        giveClassObject(Object::create(c));
    }
    
    // It is important that our default object descriptor matches that of the main class system.
    std::shared_ptr<Object> default_obj_descriptor = c_system->getDefaultObjectDescriptor();
    if (!default_obj_descriptor)
        throw std::logic_error("The base class system must have a non NULL object descriptor before initializing a validator");
        
    getClassSystem()->setDefaultObjectDescriptor(c_system->getDefaultObjectDescriptor());

    
}

Validator::~Validator()
{

}


void Validator::giveClassObject(std::shared_ptr<Object> object)
{
    if (object == NULL)
        throw std::logic_error("You must provide a non NULL object to the Validator when giving a class object. giveClassObject");
    this->class_objects.push_back(object);
}

bool Validator::isInClass()
{
    return this->current_class != NULL;
}

std::shared_ptr<Object> Validator::getClassObject(std::string name)
{
    for (std::shared_ptr<Object> obj : this->class_objects)
    {
        if (obj->getClass()->name == name)
            return obj;
    }

    return NULL;
}


void Validator::validate(Node* root_node)
{
    InterpretableNode* current_node = (InterpretableNode*) root_node;
    while(current_node != NULL)
    {
        try
        {
            current_node->test(this);
        } catch(TestError& ex)
        {
            this->logger->error(ex.what(), current_node->posInfo);
        }
        
        current_node = (InterpretableNode*) current_node->next;
    }
}

Interpreter* Validator::getInterpreter()
{
    return this->interpreter;
}

void Validator::beginClass(Class* current_class)
{
    if (this->current_class != NULL)
        throw std::logic_error("A class has already begun. End your own class: Validator::endClass();");
    this->current_class = current_class;
}

void Validator::endClass()
{
    if (this->current_class == NULL)
        throw std::logic_error("No class has been begun. You must begin a class with: Validator::beginClass(); before calling Validator::endClass();");
    this->current_class = NULL;
}

Class* Validator::getCurrentClass()
{
    return this->current_class;
}

void Validator::ignoreClass(std::string class_name)
{
    if (isClassIgnored(class_name))
        return;
    this->ignored_classes.push_back(class_name);
}

bool Validator::isClassIgnored(std::string class_name)
{
    for (std::string c : this->ignored_classes)
    {
        if (c == class_name)
            return true;
    }

    return false;
}