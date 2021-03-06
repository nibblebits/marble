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

#include "nodefactory.h"
#include "nodes.h"
#include <iostream>
NodeFactory::NodeFactory()
{

}
NodeFactory::~NodeFactory()
{
}

void NodeFactory::applyPosition(PosInfo posInfo)
{
    this->defaultPosInfo = posInfo;
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
    
    case NODE_TYPE_TRY:
    {
        node = new TryNode();
    }
    break;
    
    case NODE_TYPE_THROW:
    {
        node = new ThrowNode();
    }
    break;
    
    case NODE_TYPE_WHILE:
    {
        node = new WhileNode();
    }
    break;
    
    case NODE_TYPE_BREAK:
    {
        node = new BreakNode();
    }
    break;
    
    case NODE_TYPE_CONTINUE:
    {
        node = new ContinueNode();
    }
    break;
    
    case NODE_TYPE_DO_WHILE:
    {
        node = new DoWhileNode();
    }
    break;

    case NODE_TYPE_FOR:
    {
        node = new ForNode();
    }
    break;

    case NODE_TYPE_LIST:
    {
        node = new ListNode();
    }
    break;
    
    case NODE_TYPE_INCLUDE:
    {
        node = new IncludeNode();
    }
    break;
    
    case NODE_TYPE_INCLUDE_ONCE:
    {
        node = new IncludeOnceNode();
    }
    break;

    case NODE_TYPE_PERMISSION_NODE:
    {
        node = new PermissionNode();
    }
    break;

    case NODE_TYPE_NULL:
    {
        node = new NullNode();
    }
    break;

    case NODE_TYPE_REQUIRE:
    {
        node = new RequireNode();
    }
    break;
    
    case NODE_TYPE_BOOLEAN_LITERAL:
    {
        node = new BooleanLiteralNode();
    }
    break;

    case NODE_TYPE_OUTPUT_NODE:
    {
        node = new OutputNode();
    }
    break;
    
    case NODE_TYPE_LIMIT_SCOPE:
    {
        node = new LimitScopeNode();
    }
    break;

    case NODE_TYPE_OUTPUT_FILTER:
    {
        node = new OutputFilterNode();
    }
    break;


    case NODE_TYPE_NOT:
    {
        node = new NotNode();
    }
    break;

    default:
        throw std::logic_error("Invalid node type provided to NodeFactory::createNode");
    }

    node->posInfo = this->defaultPosInfo;
    // Lets create a unique pointer and add it to the nodes vector so the memory is freed when the node factory leaves its scope.
    this->nodes.push_back(std::unique_ptr<Node>(node));
    return node;
}


ExpNode* NodeFactory::createExpNode(ExpressionInterpretableNode* left, ExpressionInterpretableNode* right, std::string op)
{
    ExpNode* exp_node = (ExpNode*) createNode(NODE_TYPE_EXPRESSION);
    exp_node->left = left;
    exp_node->right = right;
    exp_node->op = op;
    return exp_node;
}