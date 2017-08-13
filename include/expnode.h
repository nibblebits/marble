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
	
		Node* left;
		Node* right;
		std::string op;
		bool isAssignmentOperator();
	private:
};
#endif
