#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include "node.h"
#include "token.h"
#include "statics.h"
class VarNode : public Node
{
	public:
		VarNode();
		virtual ~VarNode();
		Token* type;
		Token* name;
		Node* value;
	private:
};

#endif
