#ifndef NODEFACTORY_H
#define NODEFACTORY_H
#include <memory>
#include <vector>
#include "statics.h"
#include "posinfo.h"
/**
* This node factory will handle the creation of all nodes. Memory will be freed once the node factory leaves scope. Ensure that creation of nodes
* is not done from out of this class
*/

class Node;
class ExpNode;
class ExpressionInterpretableNode;
class NodeFactory
{
public:
    NodeFactory();
    virtual ~NodeFactory();
    void applyPosition(PosInfo posInfo);
    Node* createNode(NODE_TYPE node_type);
    ExpNode* createExpNode(ExpressionInterpretableNode* left, ExpressionInterpretableNode* right, std::string op);
private:
    std::vector<std::unique_ptr<Node>> nodes;
    PosInfo defaultPosInfo;
};

#endif
