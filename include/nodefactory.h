#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include <memory>
#include <vector>
#include "statics.h"
/**
* This node factory will handle the creation of all nodes. Memory will be freed once the node factory leaves scope. Ensure that creation of nodes
* is not done from out of this class
*/

class Node;
class NodeFactory
{
public:
    NodeFactory();
    virtual ~NodeFactory();
    Node* createNode(NODE_TYPE node_type);
private:
    std::vector<std::unique_ptr<Node>> nodes;
};

#endif
