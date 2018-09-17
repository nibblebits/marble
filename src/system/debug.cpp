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

#include "debug.h"
#include "nodes.h"
#include "permissionsobject.h"
#include "permissionobject.h"
#include <iostream>
#include <exception>

#ifdef DEBUG_ENABLED

Debug::Debug()
{

}

Debug::~Debug()
{

}

void Debug::PrintVariablesForScope(Scope* scope)
{
    if (scope == NULL)
    {
        std::cout << "NULL SCOPE" << std::endl;
        return;
    }

    std::cout << "SCOPE: " << scope << std::endl;
    for (Variable* var : scope->getVariables())
    {
        std::cout << "Variable: " << var->name << std::endl;
    }
    std::cout << "END SCOPE" << std::endl;
    if (scope->prev != NULL)
        Debug::PrintVariablesForScope(scope->prev);
}
void Debug::PrintPermissions(std::shared_ptr<PermissionsObject> permissions)
{
    std::cout << "PERMISSIONS: " << permissions << std::endl;
    for(std::shared_ptr<PermissionObject> perm : permissions->objects)
    {
        std::cout << perm->getClass()->name << std::endl;
    }
}
void Debug::OutputTabbing(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        std::cout << "\t";
    }
}
void Debug::PrintValueForNode(Node* value_node, int tabbing)
{
    if (value_node != NULL)
    {
        if (value_node->type == NODE_TYPE_LITERAL)
        {
            LiteralNode* literal_value_node = (LiteralNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS LITERAL" << std::endl;
            OutputTabbing(tabbing);
            std::cout << literal_value_node->value << std::endl;
        }
        else if(value_node->type == NODE_TYPE_IDENTIFIER)
        {
            IdentifierNode* identifier_node = (IdentifierNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS IDENTIFIER" << std::endl;
            OutputTabbing(tabbing);
            std::cout << identifier_node->value << std::endl;
        }
        else if(value_node->type == NODE_TYPE_EXPRESSION)
        {
            ExpNode* exp_node = (ExpNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS EXPRESSION" << std::endl;
            OutputTabbing(tabbing);
            std::cout << "left node: ";
            PrintValueForNode(exp_node->left, tabbing+1);
            std::cout << std::endl;
            OutputTabbing(tabbing);
            std::cout << "right node: ";
            PrintValueForNode(exp_node->right, tabbing+1);
            std::cout << std::endl;
            OutputTabbing(tabbing);
            std::cout << "operator: " << exp_node->op << std::endl;
        }
        else if(value_node->type == NODE_TYPE_CAST)
        {
            CastNode* cast_node = (CastNode*)(value_node);
            OutputTabbing(tabbing);
            std::cout << "VALUE IS CAST" << std::endl;
            OutputTabbing(tabbing);
            std::cout << "casting to: ";
            PrintValueForNode(cast_node->casting_to, tabbing+1);
            OutputTabbing(tabbing);
            std::cout << "to cast: ";
            PrintValueForNode(cast_node->to_cast, tabbing+1);
        }
	    else if(value_node->type == NODE_TYPE_NEGATIVE)
	    {
	       NegNode* neg_node = (NegNode*)(value_node);
	       OutputTabbing(tabbing);
	       std::cout << "NEG NODE" << std::endl;
           PrintValueForNode(neg_node->node, tabbing+1); 
	    }
	    else if(value_node->type == NODE_TYPE_ARRAY)
	    {
	        ArrayNode* array_node = (ArrayNode*)(value_node);
	        OutputTabbing(tabbing);
	        std::cout << "ARRAY NODE" << std::endl;
            std::cout << "Index node: ";
            PrintValueForNode(array_node->index_node, tabbing+1);
            std::cout << std::endl;
            OutputTabbing(tabbing);
            std::cout << "next node: ";
            PrintValueForNode(array_node->next_element, tabbing+1);
            std::cout << std::endl;
            OutputTabbing(tabbing);

	    }
	    else if(value_node->type == NODE_TYPE_NEW)
	    {
	        NewNode* new_node = (NewNode*)(value_node);
	        OutputTabbing(tabbing);
	        std::cout << "NEW NODE" << std::endl;
	        PrintValueForNode(new_node->type_node, tabbing+1);
	        if (new_node->isArray())
	        {
	            for (Node* node : new_node->array_values)
	            {
	                PrintValueForNode(node, tabbing+1); 
	            }
	        }
	    }
	    else if(value_node->type == NODE_TYPE_KEYWORD)
	    {
	        KeywordNode* keyword_node = (KeywordNode*) value_node;
	        OutputTabbing(tabbing);
	        std::cout << "KEYWORD NODE: " << keyword_node->value << std::endl;
	   
	    }
        else if(value_node->type == NODE_TYPE_STRING)
        {
            StringNode* string_node = (StringNode*) value_node;
            std::cout << "STRING NODE: " << string_node->value << std::endl;
        }
        else if(value_node->type == NODE_TYPE_NOT)
        {
            NotNode* not_node = (NotNode*) value_node;
            std::cout << "NOT NODE " << std::endl;
            PrintValueForNode(not_node->node, tabbing+1);
        }
        else if(value_node->type == NODE_TYPE_FUNCTION_CALL)
        {
            FunctionCallNode* fc_node = (FunctionCallNode*) value_node;
            std::cout << "FUNCTION CALL: " << fc_node->name->value << std::endl;
        } 
        else if(value_node->type == NODE_TYPE_OUTPUT_NODE)
        {
            OutputNode* output_node = (OutputNode*) value_node;
            std::cout << "OUTPUT NODE" << std::endl;
            PrintValueForNode(output_node->exp, tabbing+1);
        }
	    else
	    {
	        OutputTabbing(tabbing);
	        std::cout << "UNKNOWN DEBUG NODE TYPE: " << value_node->type << std::endl;
	    }
    }
}
#endif 
