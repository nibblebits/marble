#ifndef NODE_H
#define NODE_H
#include <memory>

class Node
{
	public:
		Node(int type);
		virtual ~Node();
		int getType();
		std::shared_ptr<Node> next;

		int type;
};

#endif
