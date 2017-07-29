#include "parser.h"
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
	if (this->root_node == NULL)
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

void Parser::parse_variable_declaration()
{
	std::shared_ptr<Token> data_type_token = this->current_token;
	std::shared_ptr<Token> var_name_token = data_type_token->next; 
	if (var_name_token == NULL)
	{
		parse_error("Variable declaration keyword provided but no variable name");
	}

	ensure_type(var_name_token, TOKEN_TYPE_IDENTIFIER);
	
	std::shared_ptr<Varnode> var_node = std::shared_ptr<Varnode>(new Varnode());
	var_node->var_type = data_type_token;
	var_node->var_name = var_name_token;
	push_node(var_node);
}

void Parser::global_parse_keyword()
{
	std::string keyword_name = this->current_token->getValue().svalue;
	if (is_datatype(keyword_name))
	{
		parse_variable_declaration();
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
		}
		this->current_token = this->current_token->next;
	}

	return this->root_node;
}


