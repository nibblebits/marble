#ifndef BODYNODE_H
#define BODYNODE_H
#include <vector>
#include <functional>
#include "inode.h"
class Interpreter;
class BodyNode : public InterpretableNode
{
public:
    BodyNode();
    virtual ~BodyNode();
    virtual Value interpret(Interpreter* interpreter);
    	
    void apply_node_listener(std::function<bool(Node* node, Value v)> node_listener_function);
	bool interpret_body_node(Node* node);
    void interpret_body(BodyNode* node);
    
    // Add's a node to the nested children
    void addChild(Node* c);
    
    // Nested children of this body node
    Node* child;
    
private:
    Interpreter* interpreter;
    std::function<bool(Node* node, Value v)> node_listener_function;
};
#endif
