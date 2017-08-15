#ifndef NODE_H
#define NODE_H
#include <memory>

class Node
{
	public:
		Node(int type);
		virtual ~Node();
		int getType();
		void setNext(Node* node);
		Node* next;
		std::unique_ptr<Node> smart_next;
		int type;
};

#endif
