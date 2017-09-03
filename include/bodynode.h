#ifndef BODYNODE_H
#define BODYNODE_H
#include <vector>
#include "node.h"
class BodyNode : public Node
{
public:
    BodyNode();
    virtual ~BodyNode();
    
    // Add's a node to the nested children
    void addChild(Node* c);
    
    // Nested children of this body node
    Node* child;
};
#endif
