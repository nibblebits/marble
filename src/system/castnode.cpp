#include "castnode.h"
#include "nodes.h"
#include <iostream>
CastNode::CastNode() : ExpressionInterpretableNode(NODE_TYPE_CAST)
{
    this->casting_to = NULL;
    this->to_cast = NULL;
}

CastNode::~CastNode()
{

}

double CastNode::get_double_value(Value* from)
{
    switch(from->type)
    {
        case VALUE_TYPE_STRING:
        {
            return std::stod(from->svalue);
        }
        
        case VALUE_TYPE_NUMBER:
        {
            // Already number
            return from->dvalue;
        }
        
        default:
            throw std::logic_error("Unable to get double value from within CastNode");
    };
}

std::string CastNode::get_string_value(Value* from)
{
    switch(from->type)
    {
        case VALUE_TYPE_STRING:
        {
            // Already string
            return from->svalue;
        };
        
        case VALUE_TYPE_NUMBER:
        {
            return std::to_string(from->dvalue);
        };
        
        default:
            throw std::logic_error("Unable to get double value from within CastNode");
    };
}

Value CastNode::interpret(Interpreter* interpreter)
{

    Value v = this->to_cast->interpret(interpreter);
    if (this->casting_to->type == NODE_TYPE_KEYWORD)
    {
        // We have a primitive cast here
        KeywordNode* keyword_node = (KeywordNode*) this->casting_to;
        std::string value = keyword_node->value;
        if (value == "int")
        {
            v.dvalue = (int) get_double_value(&v);
            v.type = VALUE_TYPE_NUMBER;
        }
        else if(value == "string")
        {
            v.svalue = get_string_value(&v);
            v.type = VALUE_TYPE_STRING;
        }
    }
 
    return v;
}

void CastNode::test(Validator* validator)
{    

}
    
