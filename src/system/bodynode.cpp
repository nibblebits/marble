/*
Marble scripting language interpreter
Copyright (C) 2018 Daniel McCarthy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "bodynode.h"
#include "inode.h"
#include "statics.h"
#include "interpreter.h"
#include "validator.h"
#include "writtenfunction.h"

BodyNode::BodyNode() : ListNode(NODE_TYPE_BODY)
{
    this->before_leave_function = NULL;
    this->node_listener_function = NULL;
    this->on_after_test_node_function = NULL;
    this->on_after_interpret_node_function = NULL;
}

BodyNode::~BodyNode()
{
}

void BodyNode::onBeforeLeave(std::function<void()> before_leave_function)
{
    this->before_leave_function = before_leave_function;
}

void BodyNode::onAfterTestNode(std::function<void(Node *node)> on_after_node_function)
{
    this->on_after_test_node_function = on_after_node_function;
}

void BodyNode::onAfterInterpretNode(std::function<void(Node *node, Value v)> on_after_interpret_node_function)
{
    this->on_after_interpret_node_function = on_after_interpret_node_function;
}

void BodyNode::test(Validator *validator, struct extras extra)
{
    test(validator, 0);
}

void BodyNode::test(Validator *validator, SCOPE_PROPERTIES scope_properties)
{
    // Let's create a new parented scope for this
    if (!(scope_properties & KEEP_SCOPE))
        validator->new_parented_scope();
    InterpretableNode *current_node = ListNode::getRootNode();
    // Awesome now lets interpret!
    while (current_node != NULL)
    {
        if (this->node_listener_function != NULL)
        {
            NODE_LISTENER_ACTION action = this->node_listener_function(current_node);
            if (action == NODE_LISTENER_ACTION_LEAVE)
                break;
            else if (action == NODE_LISTENER_ACTION_IGNORE_NODE)
            {
                // We want to ignore this node so skip to the next one
                current_node = (InterpretableNode *)current_node->next;
                continue;
            }
        }

        current_node->test(validator);
        if (this->on_after_test_node_function != NULL)
        {
            this->on_after_test_node_function(current_node);
        }
        current_node = (InterpretableNode *)current_node->next;
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

    // Finally reset monitor functions
    this->node_listener_function = NULL;
    this->on_after_test_node_function = NULL;
}

Value BodyNode::interpret(Interpreter *interpreter, struct extras extra)
{
    Value v;
    this->interpreter = interpreter;
    interpret_body(this, 0);
    return v;
}

Value BodyNode::interpret(Interpreter *interpreter, SCOPE_PROPERTIES properties, struct extras extra)
{
    Value v;
    this->interpreter = interpreter;
    interpret_body(this, properties);
    return v;
}

void BodyNode::evaluate_impl(SystemHandler *handler, EVALUATION_TYPE expected_evaluation, struct Evaluation *evaluation)
{
    throw std::logic_error("Evaluating of body nodes is not supported");
}

void BodyNode::apply_node_listener(std::function<NODE_LISTENER_ACTION(Node *node)> node_listener_function)
{
    this->node_listener_function = node_listener_function;
}

bool BodyNode::interpret_body_node(Node *node)
{
    int type = node->getType();
    InterpretableNode *inode = (InterpretableNode *)node;
    if (node_listener_function != NULL)
    {

        NODE_LISTENER_ACTION action = node_listener_function(inode);
        if (action == NODE_LISTENER_ACTION_LEAVE)
            return false;
        else if (action == NODE_LISTENER_ACTION_IGNORE_NODE)
            return true;
    }
    Value v = inode->interpret(interpreter);
    if (this->on_after_interpret_node_function != NULL)
    {
        this->on_after_interpret_node_function(inode, v);
    }

    FunctionSystem *function_system = this->interpreter->getFunctionSystem();
    if (function_system->isInFunction())
    {
        WrittenFunction *current_function = (WrittenFunction *)function_system->getCurrentFunction();
        // If this function has returned then we should stop interpreting more nodes.
        if (current_function->hasReturned())
            return false;
    }


    return true;
}

void BodyNode::finish_body(BodyNode *node, SCOPE_PROPERTIES properties)
{
    
    // Run the before leave function if one is specified
    if (before_leave_function != NULL)
    {
        before_leave_function();
        this->before_leave_function = NULL;
    }

    // We are done with this scope
    if (!(properties & KEEP_SCOPE))
        interpreter->finish_parented_scope();

    this->node_listener_function = NULL;
    this->on_after_interpret_node_function = NULL;
}

void BodyNode::interpret_body(BodyNode *node, SCOPE_PROPERTIES properties)
{
    // Let's see if we have timed out
    interpreter->checkTimeout();

    // Let's create a new parented scope for this but only if we should not keep the scope
    if (!(properties & KEEP_SCOPE))
    {
        interpreter->new_parented_scope();
    }

    Scope* my_scope = interpreter->getCurrentScope();
    // Get the root of the body. The first child.
    Node *current_node = node->getRootNode();

    try
    {
        // Awesome now lets interpret!
        while (current_node != NULL)
        {
            // Let's see if we have timed out
            interpreter->checkTimeout();
            if (!interpret_body_node(current_node))
                break;
            current_node = current_node->next;
        }
    }
    catch (...)
    {
        finish_body(node, properties);
        throw;
    }

    finish_body(node, properties);
}
