#include "keywordnode.h"
#include "statics.h"

KeywordNode::KeywordNode() : Node(NODE_TYPE_KEYWORD)
{
	this->value = "";
}

KeywordNode::~KeywordNode()
{

}
