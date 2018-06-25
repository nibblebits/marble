#include "listnode.h"
#include "interpreter.h"
ListNode::ListNode() : ListNode(NODE_TYPE_LIST)
{
    this->root = NULL;
}

ListNode::ListNode(NODE_TYPE nodeType) : InterpretableNode(nodeType)
{
    this->root = NULL;
}
ListNode::~ListNode()
{

}


void ListNode::test(Validator* validator, struct extras extra)
{
    InterpretableNode* current = this->root;
    while(current != NULL) { current->test(validator); current = (InterpretableNode*) current->next;}
}

Value ListNode::interpret(Interpreter* interpreter, struct extras extra)
{
    Value v;
    InterpretableNode* current = this->root;
    while(current != NULL)
    { 
        // Let's see if we have timed out
        interpreter->checkTimeout();
        current->interpret(interpreter); 
        current = (InterpretableNode*) current->next;
    }
    return v;
}

void ListNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Node lists do not support evaluation");
}

void ListNode::addNode(InterpretableNode* c)
{
    if (this->root == NULL)
    {
        this->root = c;
        return;
    }


    // Let's find the last current child
    Node* current = this->root;
    while(current->next != NULL) { current = current->next; }
    
    // At this point we are at the last child
    current->next = c;
}

InterpretableNode* ListNode::getRootNode()
{
    return this->root;
}