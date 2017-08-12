#ifndef NODE_H
#define NODE_H
#include <memory>

class Node
{
	public:
		Node(int type);
		virtual ~Node();
		int getType();
		Node* next;

		int type;
};

#endif
