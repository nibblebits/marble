#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "node.h"
#include "statics.h"
class ExpNode : public Node
{
	public:
		ExpNode();
		virtual ~ExpNode();
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;
		std::string op;
	private:
};
#endif
