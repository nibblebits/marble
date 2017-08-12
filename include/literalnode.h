#ifndef LITERALNODE_H
#define LITERALNODE_H

#include "node.h"
class LiteralNode : public Node
{
	public:
		LiteralNode();
		virtual ~LiteralNode();
		std::string value;
	private:
};
#endif
