#include "parser.h"
#include <iostream>

struct order_of_operation
{
	const char* op;
	int priority;
};


/* The order of operations for operators and their priorities 
 * Seek here: http://www.difranco.net/compsci/C_Operator_Precedence_Table.htm
 * The same order of operations to C will be used.*/

struct order_of_operation o_of_operation[] = {
    "<<", 0,
    ">>", 0,
    "<", 1,
    ">", 1, 
    "<=", 1,
    ">=", 1,
    "==", 2,
    "!=", 2,
    "&", 3,
    "^", 4,
    "|", 5,
    "+", 6,
    "-", 6,
    "*", 7,
    "/", 7,
    "%", 7,
    "=", 8,
    "+=", 8,
    "-=", 8,
    "*=", 8,
    "/=", 8,
    "%=", 8,
    "^=", 8,
    "&=", 8, 
    "<<=", 8,
    ">>=", 8,
};


Parser::Parser()
{
	this->root_node = NULL;	
	this->current_node = NULL;
	this->current_token = NULL;
}
Parser::~Parser()
{
}


bool Parser::is_datatype(std::string str)
{
	return str == "number" || str == "char" || str == "string" || str == "bool";
}

bool Parser::legal_value(Token* token)
{
	int token_type = token->getType();
	return token != NULL && (token_type == TOKEN_TYPE_IDENTIFIER || token_type == TOKEN_TYPE_STRING || token_type == TOKEN_TYPE_NUMBER);
}


void Parser::parse_error(std::string message)
{
	throw std::logic_error(message);
}

void Parser::ensure_type(Token* token, int expected_type)
{
	int token_type = token->getType();
	if (token_type != expected_type)
	{
		parse_error("Expecting a " + std::to_string(expected_type) + " but a " + std::to_string(token_type) + " was provided");
	}
}

void Parser::push_node(Node* node)
{
	if (this->current_node == NULL)
	{
		this->root_node = node;
		this->current_node = node;
	}
	else
	{
		this->current_node->next = node;
		this->current_node = node;
	}
}

Node* Parser::getLiteralNode(Token* token)
{
	LiteralNode* literal_node = new LiteralNode();
	literal_node->value = token->value;
	return literal_node;
}

Node* Parser::getIdentifierNode(Token* token)
{
	IdentifierNode* identifier_node = new IdentifierNode();
	return identifier_node;
}

Node* Parser::convertToNode(Token* token)
{
	int tokenType = token->getType();
	if (token->isLiteral())
	{
		return getLiteralNode(token);
	}
	else if(token->isIdentifier())
	{
		return getIdentifierNode(token);
	}

	throw std::logic_error("The single \"token\" provided cannot be converted to a node");
}

Token* Parser::peek(int ahead)
{
	return peek(this->current_token, ahead);
}

Token* Parser::peek()
{
	return this->current_token;
}

Token* Parser::peek(Token* token, int ahead)
{
	// Ok lets peek ahead and return the node, we will return NULL if there is nothing to peak ahead to.
	Token* token_to_return = token;	
	for (int i = 0; i < ahead; i++)
	{
		if (token_to_return->next == NULL)
			return NULL;

		token_to_return = token_to_return->next;
	}
	return token_to_return;

}

bool Parser::first_op_has_priority(std::string op1, std::string op2)
{
	int op1_priority = get_priority_for_op(op1);
	int op2_priority = get_priority_for_op(op2);

	return op1_priority >= op2_priority;	
}

int Parser::get_priority_for_op(std::string op)
{
	int size = sizeof (o_of_operation) / sizeof (struct order_of_operation);
	for (int i = 0; i < size; i++)
	{
		struct order_of_operation ooo = o_of_operation[i];
		if (ooo.op == op)
		{
			return ooo.priority;
		}
	}

	// Priority is not registered for this opcode? Lets just return the default priority.
	return 0;
}

Token* Parser::next()
{
	Token* next_token = this->current_token;
	if (next_token == NULL)
	{
		return NULL;
	}

	this->current_token = next_token->next;
	return next_token;
}


void Parser::parse_variable_declaration()
{
	Token* data_type_token = next();
	Token* var_name_token = next(); 
	if (var_name_token == NULL)
	{
		parse_error("Variable declaration keyword provided but no variable name");
	}

	ensure_type(var_name_token, TOKEN_TYPE_IDENTIFIER);
	Varnode* var_node = new Varnode();
	var_node->type = data_type_token;
	var_node->name = var_name_token;
	Token* token_ahead = peek();
	if (token_ahead->isOperator("="))
	{
		// We don't need the equals operator anymore
		next();
		parse_expression();
		var_node->value = pop_node();
	}
	push_node(var_node);

	parse_semicolon();

}

void Parser::parse_value()
{
	Token* token = next();
	Node* node = NULL;
	// Do we have a nested expression here?
	if (token->isSymbol("("))
	{
		// Yes we have an expression lets process it
		parse_expression();

		// Now we must get rid of the expression terminator ")"
		token = next();
		if (!token->isSymbol(")"))
		{
			parse_error("Expecting an expression terminator for the given expression");
		}

		node = pop_node();	
	}
	else
	{
		if (!legal_value(token))
		{
			parse_error("Expecting a legal value");
		}
		node = convertToNode(token);
	}

	push_node(node);
}


void Parser::parse_semicolon()
{
	Token*  token = next();
	if (token == NULL || !token->isSymbol(";"))
	{
		parse_error("Expecting a semicolon");
	}
}

Node* Parser::get_node_before_last()
{
	Node* node = this->root_node;
	while(node != NULL)
	{
		if (node->next != NULL && node->next->next == NULL)
		{
			return node;
		}

		node = node->next;
	}

	return NULL;
}

Node* Parser::pop_node()
{
	Node* node = get_node_before_last();
	Node* node_to_return = this->current_node;
	if (node != NULL)
    {
		node->next = NULL;
	}
	
	this->current_node = node;	
	return node_to_return;
}

void Parser::parse_expression()
{
	parse_expression_part();
	Token* peeked_token = peek();
	if (peeked_token != NULL && peeked_token->isOperator())
	{
		// Lets remove the operator from the token stream
		std::string op = next()->value;

		// We now need the last expression as it needs to become our left parameter
		Node* left = pop_node();
		ExpNode* left_exp = (ExpNode*)(left);

		// We got more to go!
		parse_expression();

		Node* right = pop_node();
		if (right->type != NODE_TYPE_EXPRESSION)
		{
			if (!first_op_has_priority(left_exp->op, op))
			{
				Node* right_of_left = left_exp->right;
				ExpNode* exp_node = new ExpNode();
				exp_node->left = right_of_left;
				exp_node->right = right;
				exp_node->op = op;
				left = left_exp->left;
				right = exp_node;
				op = left_exp->op;
			}
		}

		ExpNode* exp_node = new ExpNode();
		exp_node->left = left;
		exp_node->right = right;
		exp_node->op = op;
		push_node(exp_node);
	}
}

void Parser::parse_expression_part()
{
	// Parse the left value
	parse_value();
	Node* exp_left = pop_node();
	Node* node = exp_left;
	Token* peeked_token = peek();
	if (peeked_token != NULL && peeked_token->isOperator())
	{
		// We have a right part of the expression "l + r"
		std::string op = next()->value;
		// Lets parse the right value
		parse_value();
		Node* exp_right = pop_node();
		ExpNode* exp_node = new ExpNode();
		exp_node->left = exp_left;
		exp_node->right = exp_right;
		exp_node->op = op;
		node = exp_node;
	}
	push_node(node);

}

void Parser::global_parse_keyword()
{
	std::string keyword_name = this->current_token->getValue();
	if (is_datatype(keyword_name))
	{
		// Either this is a function or a variable declaration
		Token* peeked_token = peek(this->current_token, 2);
		// If the peeked token is a symbol then this must be a function, e.g number a() { } otherwise its a variable declaration
		if (peeked_token != NULL && peeked_token->isSymbol("("))
		{
		//	parse_function();
		}
		else
		{
			parse_variable_declaration();
		}
	}
}

Node* Parser::parse(Token* root_token)
{
	this->root_node = NULL;	
	this->current_node = NULL;
	this->current_token = root_token;
	while(this->current_token != NULL)
	{
		int type = this->current_token->getType();
		switch(type)
		{
			case TOKEN_TYPE_KEYWORD:
			{
				global_parse_keyword();
			}
			break;
			default:
				parse_error("Unexpected token");
		}
	}
	return this->root_node;
}


