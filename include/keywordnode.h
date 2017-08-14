#ifndef KEYWORDNODE_H
#define KEYWORDNODE_H
#include "node.h"
#include <string>
class KeywordNode : public Node
{
	public:
		KeywordNode();
		virtual ~KeywordNode();
		std::string value;
};

#endif
