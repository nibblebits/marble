#ifndef LISTNODE_H
#define LISTNODE_H
#include "inode.h"
class Validator;
class ListNode : public InterpretableNode
{
public:
    ListNode();
    ListNode(NODE_TYPE nodeType);
    virtual ~ListNode();

    virtual void test(Validator* validator, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, struct extras extra={});
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);

     // Add's a node to the nested children
    void addNode(InterpretableNode* c);
    InterpretableNode* getRootNode();
private:
    // The root node of this list
    InterpretableNode* root;
};
#endif
