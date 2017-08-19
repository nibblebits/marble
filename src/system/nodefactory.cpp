#include "nodefactory.h"
#include "nodes.h"
NodeFactory::NodeFactory()
{

}
NodeFactory::~NodeFactory()
{

}

Node* NodeFactory::createNode(NODE_TYPE node_type)
{
    Node* node;
    switch(node_type)
    {
    case NODE_TYPE_VARIABLE_DECLARATION:
    {
        node = new VarNode();
    }
    break;

    case NODE_TYPE_EXPRESSION:
    {
        node = new ExpNode();
    }
    break;

    case NODE_TYPE_LITERAL:
    {
        node = new LiteralNode();
    }
    break;

    case NODE_TYPE_IDENTIFIER:
    {
        node = new IdentifierNode();
    }
    break;

    case NODE_TYPE_KEYWORD:
    {
        node = new KeywordNode();
    }
    break;

    case NODE_TYPE_FUNCTION_CALL:
    {
        node = new FunctionCallNode();
    }
    break;

    default:
        throw std::logic_error("Invalid node type provided to NodeFactory::createNode");
    }

    // Lets create a unique pointer and add it to the nodes vector so the memory is freed when the node factory leaves its scope.
    this->nodes.push_back(std::unique_ptr<Node>(node));
    return node;
}
