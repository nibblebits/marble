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

void Debug::PrintValueForNode(std::shared_ptr<Node> value_node)
{
	if (value_node != NULL)
	{
			if (value_node->type == NODE_TYPE_LITERAL)
			{
				std::shared_ptr<LiteralNode> literal_value_node = std::dynamic_pointer_cast<LiteralNode>(value_node);
				std::cout << "VALUE IS LITERAL" << std::endl;
				std::cout << literal_value_node->value.svalue << std::endl;
			}
			else if(value_node->type == NODE_TYPE_EXPRESSION)
			{
				std::shared_ptr<ExpNode> exp_node = std::dynamic_pointer_cast<ExpNode>(value_node);
				std::cout << "VALUE IS EXPRESSION" << std::endl;
				std::cout << "left node: ";
				PrintValueForNode(exp_node->left);
				std::cout << std::endl;
				std::cout << "right node: ";
				PrintValueForNode(exp_node->right);
				std::cout << std::endl;
				std::cout << "operator: " << exp_node->op << std::endl;
			}
	}
}
#endif
