#include "parser.h"
#include <iostream>
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

bool Parser::legal_value(std::shared_ptr<Token> token)
{
	int token_type = token->getType();
	return token != NULL && (token_type == TOKEN_TYPE_IDENTIFIER || token_type == TOKEN_TYPE_STRING || token_type == TOKEN_TYPE_NUMBER);
}


void Parser::parse_error(std::string message)
{
	throw std::logic_error(message);
}

void Parser::ensure_type(std::shared_ptr<Token> token, int expected_type)
{
	int token_type = token->getType();
	if (token_type != expected_type)
	{
		parse_error("Expecting a " + std::to_string(expected_type) + " but a " + std::to_string(token_type) + " was provided");
	}
}

void Parser::push_node(std::shared_ptr<Node> node)
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

std::shared_ptr<Node> Parser::getLiteralNode(std::shared_ptr<Token> token)
{
	std::shared_ptr<LiteralNode> literal_node = std::shared_ptr<LiteralNode>(new LiteralNode());
	literal_node->value = token->value;
	return literal_node;
}

std::shared_ptr<Node> Parser::getIdentifierNode(std::shared_ptr<Token> token)
{
	std::shared_ptr<IdentifierNode> literal_node = std::shared_ptr<IdentifierNode>(new IdentifierNode());
}

std::shared_ptr<Node> Parser::convertToNode(std::shared_ptr<Token> token)
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

std::shared_ptr<Token> Parser::peek(int ahead)
{
	return peek(this->current_token, ahead);
}

std::shared_ptr<Token> Parser::peek()
{
	return this->current_token;
}

std::shared_ptr<Token> Parser::peek(std::shared_ptr<Token> token, int ahead)
{
	// Ok lets peek ahead and return the node, we will return NULL if there is nothing to peak ahead to.
	std::shared_ptr<Token> token_to_return = token;	
	for (int i = 0; i < ahead; i++)
	{
		if (token_to_return->next == NULL)
			return NULL;

		token_to_return = token_to_return->next;
	}
	return token_to_return;

}

std::shared_ptr<Token> Parser::next()
{
	std::shared_ptr<Token> next_token = this->current_token;
	this->current_token = next_token->next;
	return next_token;
}


void Parser::parse_variable_declaration()
{
	std::shared_ptr<Token> data_type_token = next();
	std::shared_ptr<Token> var_name_token = next(); 
	if (var_name_token == NULL)
	{
		parse_error("Variable declaration keyword provided but no variable name");
	}

	ensure_type(var_name_token, TOKEN_TYPE_IDENTIFIER);
	std::shared_ptr<Varnode> var_node = std::shared_ptr<Varnode>(new Varnode());
	var_node->type = data_type_token;
	var_node->name = var_name_token;
	std::shared_ptr<Token> token_ahead = peek();
	if (token_ahead->isOperator("="))
	{
		// We don't need the equals operator anymore
		next();
		parse_expression();
		var_node->value = pop_node();
	}
	push_node(var_node);

}

void Parser::parse_value()
{
	std::shared_ptr<Token> token = next();
	if (!legal_value(token))
	{
		parse_error("Expecting a legal value");
	}
	std::shared_ptr<Node> value_node = convertToNode(token);
	push_node(value_node);
}

std::shared_ptr<Node> Parser::get_node_before_last()
{
	std::shared_ptr<Node> node = this->root_node;
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

std::shared_ptr<Node> Parser::pop_node()
{
	std::shared_ptr<Node> node = get_node_before_last();
	std::shared_ptr<Node> node_to_return = this->current_node;
	if (node != NULL)
    {
		node->next = NULL;
	}
	
	this->current_node = node;	
	return node_to_return;
}

void Parser::parse_expression()
{
	// Parse the left value
	parse_value();
	std::shared_ptr<Node> exp_left = pop_node();
	push_node(exp_left);
	//std::shared_ptr<Expnode> exp_node = std::shared_ptr<Expnode>(new Expnode());
	//exp_node->left = left_node;
	//exp_node->right = right_node;
	//push_node(exp_node);

}

void Parser::global_parse_keyword()
{
	std::string keyword_name = this->current_token->getValue().svalue;
	if (is_datatype(keyword_name))
	{
		// Either this is a function or a variable declaration
		std::shared_ptr<Token> peeked_token = peek(this->current_token, 2);
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

std::shared_ptr<Node> Parser::parse(std::shared_ptr<Token> root_token)
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

		if (this->current_token != NULL)
		{
			this->current_token = this->current_token->next;
		}
	}
	return this->root_node;
}


