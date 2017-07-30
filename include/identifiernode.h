#ifndef IDENTIFIERNODE_H
#define IDENTIFIERNODE_H
#include "node.h"
#include <string>
class IdentifierNode : public Node
{
	public:
		IdentifierNode();
		virtual ~IdentifierNode();
		std::string value;
};

#endif
