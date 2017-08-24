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
    }
}
#endif
