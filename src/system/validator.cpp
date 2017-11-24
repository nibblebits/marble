#include "validator.h"
#include "nodes.h"
#include "logger.h"
#include "object.h"
#include "class.h"
#include <iostream>
#include "exceptions/testerror.h"
Validator::Validator(Logger* logger, ClassSystem* baseClassSystem, FunctionSystem* baseFunctionSystem) : SystemHandler(SYSTEM_HANDLER_VALIDATOR, baseClassSystem, baseFunctionSystem)
{
    this->logger = logger;
    this->current_class = NULL;
    
    // We must create class objects for all current classes so that they will be compatible with the validation system
    for (Class* c : baseClassSystem->getAllClasses())
    {
        giveClassObject(Object::create(c));
    }
    
    // It is important that our default object descriptor matches that of the main class system.
    std::shared_ptr<Object> default_obj_descriptor = baseClassSystem->getDefaultObjectDescriptor();
    if (!default_obj_descriptor)
        throw std::logic_error("The base class system must have a non NULL object descriptor before initializing a validator");
        
    getClassSystem()->setDefaultObjectDescriptor(baseClassSystem->getDefaultObjectDescriptor());
    
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

Object* Validator::getClassObject(std::string name)
{
    for (std::shared_ptr<Object> obj : this->class_objects)
    {
        if (obj->getClass()->name == name)
            return obj.get();
    }

    return NULL;
}

void Validator::save()
{
    this->rules_stack.push_back(this->rules);
    struct rules r;
    this->rules = r;
}

void Validator::restore()
{
    this->rules = this->rules_stack.back();
    this->rules_stack.pop_back();
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


void Validator::expectingArray(int dimensions)
{
    this->rules.expected_array_dimensions = dimensions;
}

void Validator::expecting(std::string type)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");
    this->rules.expecting_type = type;
    this->rules.expecting_value_type = Value::getValueTypeForString(type);
    this->rules.expecting_variable_type = Variable::getVariableTypeForString(type);
}

bool Validator::isExpecting()
{
    return this->rules.expecting_type != "";
}

bool Validator::isExpectingArray()
{
    return this->rules.expected_array_dimensions != 0;
}

int Validator::getExpectedArrayDimensions()
{
    return this->rules.expected_array_dimensions;
}

void Validator::endExpecting()
{
    this->rules.expecting_type = "";
    this->rules.expected_array_dimensions = 0;
}

std::string Validator::getExpectingType()
{
    return this->rules.expecting_type;
}

VALUE_TYPE Validator::getExpectingValueType()
{
    return this->rules.expecting_value_type;
}

VARIABLE_TYPE Validator::getExpectingVariableType()
{
    return this->rules.expecting_variable_type;
}
