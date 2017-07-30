#ifndef LITERALNODE_H
#define LITERALNODE_H

#include "node.h"
#include "value.h"
class LiteralNode : public Node
{
	public:
		LiteralNode();
		virtual ~LiteralNode();
		struct value value;
	private:
};
#endif
