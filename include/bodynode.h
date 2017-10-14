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
    virtual void test(Validator* validator);
    virtual Value interpret(Interpreter* interpreter);
    
    void onBeforeLeave(std::function<void()> before_leave_function);
    void apply_node_listener(std::function<bool(Node* node, Value v)> node_listener_function);
	bool interpret_body_node(Node* node);
    void interpret_body(BodyNode* node);
    
    // Add's a node to the nested children
    void addChild(InterpretableNode* c);
    
    // Nested children of this body node
    InterpretableNode* child;
    
private:
    Interpreter* interpreter;
    std::function<bool(Node* node, Value v)> node_listener_function;
    std::function<void()> before_leave_function;
};
#endif
