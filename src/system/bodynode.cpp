#include "bodynode.h"
#include "statics.h"
BodyNode::BodyNode() : Node(NODE_TYPE_BODY)
{
    this->child = NULL;
}

BodyNode::~BodyNode()
{

}

void BodyNode::addChild(Node* c)
{
    if (this->child == NULL)
    {
        this->child = c;
        return;
    }


    // Let's find the last current child
    Node* current = this->child;
    while(current->next != NULL) { current = current->next; }
    
    // At this point we are at the last child
    current->next = c;
}
