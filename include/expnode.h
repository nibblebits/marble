#ifndef EXPNODE_H
#define EXPNODE_H

#include <memory>
#include "node.h"
#include "statics.h"
class Expnode : public Node
{
	public:
		Expnode();
		virtual ~Expnode();
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;
	private:
};
#endif
