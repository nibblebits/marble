#include "validator.h"
#include "nodes.h"
#include "logger.h"
#include "object.h"
Validator::Validator(Logger* logger) : SystemHandler(SYSTEM_HANDLER_VALIDATOR)
{
    this->logger = logger;
    this->current_class = NULL;
}

Validator::~Validator()
{

}


void Validator::giveClassObject(std::shared_ptr<Object> object)
{
    this->class_objects.push_back(object);
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
        } catch(std::logic_error& ex)
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



void Validator::expectingObject(std::string obj_name)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");

    this->rules.expecting_type = VALUE_TYPE_OBJECT;
    this->rules.expecting_object = obj_name;
}

void Validator::expecting(VALUE_TYPE type)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");
    this->rules.expecting_type = type;
}

bool Validator::isExpecting()
{
    return this->rules.expecting_type != -1;
}

void Validator::endExpecting()
{
    this->rules.expecting_type = -1;
    this->rules.expecting_object = "";
}

VALUE_TYPE Validator::getExpectingType()
{
    return this->rules.expecting_type;
}

std::string Validator::getExpectingObject()
{
    return this->rules.expecting_object;
}
