
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