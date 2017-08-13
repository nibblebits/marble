#include <iostream>
#include <memory.h>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include "interpreter.h"
#include "splitter.h"
#include "lexer.h"
#include "parser.h"
#include "nodes.h"
#include "debug.h"
#include "scope.h"
#include "variable.h"
#include "exceptions/IOException.h"
Interpreter::Interpreter()
{

}

Interpreter::~Interpreter()
{

}

void Interpreter::output(const char* data)
{
    /* Here a special output function would be called 
     * instead of direct I/O. This would allow the user of the Marble library to determine how output should be handled. For example if this is running on a standalone application it should output to the console, or if its a web application it should output to the web client of the browsing user.*/
	std::cout << data << std::endl;
}

void Interpreter::run(const char* code)
{
    Lexer lexer;
    lexer.setInput(code, strlen(code));
    Token* root_token = lexer.lex();
	Token* token = root_token;
	while(token != NULL)
	{
		std::cout << token->getType() << ": " << token->getValue() << std::endl;
		token = token->next;	
	}

	Parser parser;
	Node* root_node = parser.parse(root_token);
	Node* current_node = root_node;

	// Awesome now lets interpret!
	while(current_node != NULL)
	{
		int type = current_node->getType();
		switch (type)
		{
			case NODE_TYPE_VARIABLE_DECLARATION:
			{
				interpret_variable_node((VarNode*) current_node);
			}
			break;
			case NODE_TYPE_EXPRESSION:
			{
				interpret_expression((ExpNode*)current_node);
			}
			break;			
		}
	
		current_node = current_node->next;
	}

	for (Variable* variable : root_scope.getVariables())
	{
		std::cout << "Variable: " << variable->name << " = " << variable->dvalue << std::endl;
	}
}


void Interpreter::interpret_assignment_expression(ExpNode* exp_node)
{
	IdentifierNode* dest_node = (IdentifierNode*) exp_node->left;
	Node* value_node = exp_node->right;	
	Variable* variable = root_scope.getVariable(dest_node->value);
	if (variable->type == VARIABLE_TYPE_NUMBER)
	{
		variable->dvalue = evaluate_expression_get_number(value_node);
	}

}

void Interpreter::interpret_expression(ExpNode* exp_node)
{
	Node* left = exp_node->left;
	Node* right = exp_node->right;
	
	if (exp_node->isAssignmentOperator())
	{
		// We have an assignment expression
		interpret_assignment_expression(exp_node);
		return;
	}

	// There is no need to interpret anything other than assignments as we do not return a value. We wont throw an error but just let it be
}

void Interpreter::interpret_variable_node(VarNode* var_node)
{
	Token* type_token = var_node->type;
	Token* name_token = var_node->name;
	Node* value_node = var_node->value;

	Variable* variable = new Variable();
	variable->name = name_token->value;
	if (type_token->isKeyword("number"))
	{
		variable->type = VARIABLE_TYPE_NUMBER;
		variable->dvalue = evaluate_expression_get_number(value_node);
		root_scope.registerVariable(variable);
	}
}

double Interpreter::evaluate_expression_get_number(Node* node)
{
	Node* current_node = node;
	switch(current_node->type)
	{
		case NODE_TYPE_LITERAL:
		{
			LiteralNode* literal_node = (LiteralNode*) node;
			return std::stod(literal_node->value);
		}
		break;

		case NODE_TYPE_EXPRESSION:
		{
			ExpNode* exp_node = (ExpNode*) node;
			Node* left = exp_node->left;
			Node* right = exp_node->right;
			double l_value = evaluate_expression_get_number(left);
			double r_value = evaluate_expression_get_number(right);
			return op_on_values(l_value, r_value, exp_node->op);
		}
		break;
	}
}

double Interpreter::op_on_values(double value1, double value2, std::string op)
{
	if (op == "+")
	{
		return value1 + value2;
	}
	else if(op == "-")
	{
		return value1 - value2;
	}
	else if(op == "*")
	{
		return value1 * value2;
	}
	else if(op == "/")
	{
		return value1 / value2;
	}
	else
	{
		throw std::logic_error("Unexpected operator: " + op);
	}
}
void Interpreter::runScript(const char* filename)
{
   // Lets load this script
   FILE* file = fopen(filename, "r");
   if (!file)
   {
     throw IOException("Failed to open file: " + std::string(filename));
   }

   if(fseek(file, 0, SEEK_END) != 0)
   {
     throw IOException("Failed to seek to the end of the file: " + std::string(filename));
   }

   long data_len = ftell(file);
   rewind(file);
   char* data = new char[data_len];
   fread(data, data_len, 1, file);

   Splitter splitter;
   splitter.setData(data, data_len);
   
   split split;
   while(splitter.split(&split))
   {
	// Output the data
    if (split.has_data)
    {
        char* output_data = new char[split.output.size+1];
        memcpy(output_data, split.output.data, split.output.size);
        output_data[split.output.size] = 0;
        output(output_data);
        delete output_data;
    }
	
	// Run the code
    if (split.has_code)
    {
	    char* code_data = new char[split.code.size+1];
	    memcpy(code_data, split.code.data, split.code.size);
	    code_data[split.code.size] = 0;
	    run(code_data);
	    delete code_data;
    }
   }

    // Close and clean up
    fclose(file);
    delete data;
}

