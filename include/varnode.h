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
		std::shared_ptr<Token> type;
		std::shared_ptr<Token> name;
		std::shared_ptr<Node> value;
	private:
};

#endif
