#include "validator.h"
#include "nodes.h"
#include "logger.h"
Validator::Validator(Logger* logger)
{
    this->logger = logger;
    this->expecting_type = -1;
    this->expecting_object = "";
}

Validator::~Validator()
{

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
            PosInfo posInfo;
            this->logger->error(ex.what(), posInfo);
        }
        current_node = (InterpretableNode*) current_node->next;
    }
}

void Validator::expectingObject(std::string obj_name)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");
    
    this->expecting_type = VALUE_TYPE_OBJECT;
    this->expecting_object = obj_name;
}

void Validator::expecting(VALUE_TYPE type)
{
    if (isExpecting())
        throw std::logic_error("The validator is already expecting");
    this->expecting_type = type;
}

bool Validator::isExpecting()
{
    return this->expecting_type != -1;
}

void Validator::endExpecting()
{
    this->expecting_type = -1;
    this->expecting_object = "";
}

VALUE_TYPE Validator::getExpectingType()
{
    return this->expecting_type;
}

std::string Validator::getExpectingObject()
{
    return this->expecting_object;
}
