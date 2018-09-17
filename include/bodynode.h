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

#ifndef BODYNODE_H
#define BODYNODE_H
#include <vector>
#include <functional>
#include "listnode.h"
#include "statics.h"
class Interpreter;
class BodyNode : public ListNode
{
public:
    BodyNode();
    virtual ~BodyNode();
    virtual void test(Validator* validator, struct extras extra={});
    void test(Validator* validator, SCOPE_PROPERTIES scope_properties);
    virtual Value interpret(Interpreter* interpreter, struct extras extra={});
    virtual Value interpret(Interpreter* interpreter, SCOPE_PROPERTIES properties, struct extras extra={});
    virtual void evaluate_impl(SystemHandler* handler, EVALUATION_TYPE expected_evaluation, struct Evaluation* evaluation);
    
    void onBeforeLeave(std::function<void()> before_leave_function);
    void onAfterTestNode(std::function<void(Node* node)> on_after_test_node_function);
    void onAfterInterpretNode(std::function<void(Node* node, Value v)> on_after_interpret_node_function);
    void apply_node_listener(std::function<NODE_LISTENER_ACTION(Node* node)> node_listener_function);
	bool interpret_body_node(Node* node);
    void interpret_body(BodyNode* node, SCOPE_PROPERTIES properties);
    void finish_body(BodyNode* node, SCOPE_PROPERTIES properties);

    
private:
    Interpreter* interpreter;
    std::function<NODE_LISTENER_ACTION(Node* node)> node_listener_function;
    std::function<void()> before_leave_function;
    std::function<void(Node* node)> on_after_test_node_function;
    std::function<void(Node* node, Value v)> on_after_interpret_node_function;
};
#endif
