#ifndef VARNODE_H
#define VARNODE_H
#include <memory>
#include "node.h"
#include "token.h"
#include "statics.h"
class Varnode : public Node
{
	public:
		Varnode();
		virtual ~Varnode();
		Token* type;
		Token* name;
		Node* value;
	private:
};

#endif
