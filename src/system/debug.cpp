#include "debug.h"
#include "nodes.h"
#include <iostream>

#ifdef DEBUG_ENABLED

Debug::Debug()
{

}

Debug::~Debug()
{

}

void Debug::OutputTabbing(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        std::cout << "\t";
    }
}
void Debug::PrintValueForNode(Node* value_node, int tabbing)
{
    if (value_node != NULL)
    {

        if (value_node->type == NODE_TYPE_LITERAL)
        {
            LiteralNode* literal_value_node = (LiteralNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS LITERAL" << std::endl;
            OutputTabbing(tabbing);
            std::cout << literal_value_node->value << std::endl;
        }
        else if(value_node->type == NODE_TYPE_IDENTIFIER)
        {
            IdentifierNode* identifier_node = (IdentifierNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS IDENTIFIER" << std::endl;
            OutputTabbing(tabbing);
            std::cout << identifier_node->value << std::endl;
        }
        else if(value_node->type == NODE_TYPE_EXPRESSION)
        {
            ExpNode* exp_node = (ExpNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS EXPRESSION" << std::endl;
            OutputTabbing(tabbing);
            std::cout << "left node: ";
            PrintValueForNode(exp_node->left, tabbing+1);
            std::cout << std::endl;
            OutputTabbing(tabbing);
            std::cout << "right node: ";
            PrintValueForNode(exp_node->right, tabbing+1);
            std::cout << std::endl;
            OutputTabbing(tabbing);
            std::cout << "operator: " << exp_node->op << std::endl;
        }
        else if(value_node->type == NODE_TYPE_CAST)
        {
            CastNode* cast_node = (CastNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS CAST" << std::endl;
            OutputTabbing(tabbing);
            std::cout << "casting to: ";
            PrintValueForNode(cast_node->casting_to, tabbing+1);
            OutputTabbing(tabbing);
            std::cout << "to cast: ";
            PrintValueForNode(cast_node->to_cast, tabbing+1);
        }
	    else if(value_node->type == NODE_TYPE_NEGATIVE)
	    {
	       NegNode* neg_node = (NegNode*)(value_node);
	       OutputTabbing(tabbing);
	       std::cout << "NEG NODE" << std::endl;
           PrintValueForNode(neg_node->node, tabbing+1); 
	    }
	    else
	    {
	        std::cout << "UNKNOWN DEBUG NODE TYPE: " << value_node->type << std::endl;
	    }
    }
}
#endif 
