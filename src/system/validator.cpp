#include "validator.h"
#include "nodes.h"
#include "logger.h"
Validator::Validator(Logger* logger)
{
    this->logger = logger;
}

Validator::~Validator()
{

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
