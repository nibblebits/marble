#include "bodynode.h"
#include "inode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "writtenfunction.h"

BodyNode::BodyNode() : InterpretableNode(NODE_TYPE_BODY)
{
    this->child = NULL;
    this->before_leave_function = NULL;
    this->node_listener_function = NULL;
    this->on_after_test_node_function = NULL;
}

BodyNode::~BodyNode()
{

}


void BodyNode::onBeforeLeave(std::function<void()> before_leave_function)
{
    this->before_leave_function = before_leave_function;
}

void BodyNode::onAfterTestNode(std::function<void(Node* node)> on_after_test_node_function)
{
    this->on_after_test_node_function = on_after_test_node_function;
}

void BodyNode::test(Validator* validator)
{
    test(validator, 0);
}

void BodyNode::test(Validator* validator, SCOPE_PROPERTIES scope_properties)
{
    // Let's create a new parented scope for this
    if (!(scope_properties & KEEP_SCOPE))
        validator->new_parented_scope();
    InterpretableNode* current_node = child;
    // Awesome now lets interpret!
    while(current_node != NULL)
    {
        current_node->test(validator);
        if (this->on_after_test_node_function != NULL)
        {
            this->on_after_test_node_function(current_node);
        }
        current_node = (InterpretableNode*) current_node->next;
    }
    
    // Run the before leave function if one is specified
    if (before_leave_function != NULL)
    {
        before_leave_function();
        this->before_leave_function = NULL;
    }
    
    // We are done with this scope
    if (!(scope_properties & KEEP_SCOPE))
        validator->finish_parented_scope(); 
}


Value BodyNode::interpret(Interpreter* interpreter)
{
    Value v;
    this->interpreter = interpreter;
    interpret_body(this);
    return v;
}

void BodyNode::evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation)
{
    throw std::logic_error("Evaluating of body nodes is not supported");
}

void BodyNode::apply_node_listener(std::function<bool(Node* node, Value v)> node_listener_function)
{
    this->node_listener_function = node_listener_function;
}

bool BodyNode::interpret_body_node(Node* node)
{
    int type = node->getType();
    InterpretableNode* inode = (InterpretableNode*) node;
    Value v = inode->interpret(interpreter);
    if (node_listener_function != NULL && (!node_listener_function(inode, v))) 
        return false;
    
    FunctionSystem* function_system = this->interpreter->getFunctionSystem();
    if (function_system->isInFunction())
    {
        WrittenFunction* current_function = (WrittenFunction*) function_system->getCurrentFunction();
        // If this function has returned then we should stop interpreting more nodes.
        if (current_function->hasReturned())
            return false;
    }
    return true;
}

void BodyNode::interpret_body(BodyNode* node)
{
    // Let's create a new parented scope for this
    interpreter->new_parented_scope();
    Node* current_node = node->child;

    // Awesome now lets interpret!
    while(current_node != NULL)
    {
        if(!interpret_body_node(current_node)) goto end;
            current_node = current_node->next;
    }

end:

    // Run the before leave function if one is specified
    if (before_leave_function != NULL)
    {
        before_leave_function();
        this->before_leave_function = NULL;
    }
    
    // We are done with this cope
    interpreter->finish_parented_scope(); 
    this->node_listener_function = NULL;
    return;
}

void BodyNode::addChild(InterpretableNode* c)
{
    if (this->child == NULL)
    {
        this->child = c;
        return;
    }


    // Let's find the last current child
    Node* current = this->child;
    while(current->next != NULL) { current = current->next; }
    
    // At this point we are at the last child
    current->next = c;
}
