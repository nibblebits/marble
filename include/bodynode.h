#ifndef BODYNODE_H
#define BODYNODE_H
#include <vector>
#include <functional>
#include "listnode.h"
#include "breakable.h"
class Interpreter;
class BodyNode : public ListNode, public Breakable
{
public:
    BodyNode();
    virtual ~BodyNode();
    virtual void test(Validator* validator, struct extras extra);
    void test(Validator* validator, SCOPE_PROPERTIES scope_properties);
    virtual Value interpret(Interpreter* interpreter, struct extras extra);
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    virtual void didBreak(BREAK_TYPE type);
    
    void onBeforeLeave(std::function<void()> before_leave_function);
    void onAfterTestNode(std::function<void(Node* node)> on_test_node_function);
    void apply_node_listener(std::function<bool(Node* node, Value v)> node_listener_function);
	bool interpret_body_node(Node* node);
    void interpret_body(BodyNode* node);
    
    
private:
    Interpreter* interpreter;
    std::function<bool(Node* node, Value v)> node_listener_function;
    std::function<void()> before_leave_function;
    std::function<void(Node* node)> on_after_test_node_function;
};
#endif
