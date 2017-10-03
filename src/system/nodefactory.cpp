#include "nodefactory.h"
#include "nodes.h"
#include <iostream>
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

    case NODE_TYPE_STRING:
    {
        node = new StringNode();
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

    case NODE_TYPE_NEGATIVE:
    {
        node = new NegNode();
    }
    break;
    
    case NODE_TYPE_IF_STMT:
    {
        node = new IfStatementNode();
    }
    break;
    
    case NODE_TYPE_BODY:
    {
        node = new BodyNode();
    }
    break;
    
    case NODE_TYPE_CAST:
    {
        node = new CastNode();
    }
    break;
    
    case NODE_TYPE_ARRAY:
    {
        node = new ArrayNode();
    }
    break;
    
    case NODE_TYPE_NEW:
    {
        node = new NewNode();
    }
    break;
    case NODE_TYPE_FUNCTION:
    {
        node = new FunctionNode();
    }
    break;   
    case NODE_TYPE_RETURN:
    {
        node = new ReturnNode();
    }
    break;
    
    case NODE_TYPE_CLASS:
    {
        node = new ClassNode();
    }
    break;
    
    default:
        throw std::logic_error("Invalid node type provided to NodeFactory::createNode");
    }

    // Lets create a unique pointer and add it to the nodes vector so the memory is freed when the node factory leaves its scope.
    this->nodes.push_back(std::unique_ptr<Node>(node));
    return node;
}
