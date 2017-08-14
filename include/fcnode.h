#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include "node.h"
#include <vector>
class FunctionCallNode : public Node
{
	public:
		FunctionCallNode();
		virtual ~FunctionCallNode();
		Node* dest;
		std::vector<Node*> arguments;
};
#endif
