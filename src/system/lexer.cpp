#include "lexer.h"
#include <vector>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <memory>
#include "statics.h"
#include "config.h"
const char keywords[][MAX_KEYWORD_SIZE] = {"public", "private", "protected", "number", "char", "string", "bool", "class", "return", "continue", "break", "void"};
const char valid_operators[][MAX_OPERATORS_SIZE] = {"+", "-", "*", "/", "++", "--", "+=", "-=", "/=", "*=", "-=", "="};
const char symbols[] = {';',',','(', ')', '{', '}','[',']'};
Lexer::Lexer()
{
	this->root = NULL;
}
Lexer::~Lexer()
{
	cleanup();
}

void Lexer::cleanup()
{
	if (this->root != NULL)
	{
		Token* current = this->root;
		do
		{
			Token* next = current->next;
			delete current;
			current = next;
		} while(current != NULL);
	}
}
void Lexer::setInput(const char* buf, int size)
{
	this->buf = buf;
	this->size = size;
	this->end = this->buf+this->size;	
}

bool Lexer::bounds_safe(const char* ptr)
{
	return ptr < this->end;
}

bool Lexer::is_keyword(std::string value)
{
	int total_keywords = sizeof(keywords) / MAX_KEYWORD_SIZE;
	for (int i = 0; i < total_keywords; i++)
	{
		const char* keyword = keywords[i];
		if (value == keyword)
		{
			return true;
		}
	}

	return false;
}

bool Lexer::is_whitespace(char c)
{
	return c < 33;
}

bool Lexer::is_operator(char c)
{
	std::string c_str = "";
	c_str += c;
	return is_operator(c_str);
}

bool Lexer::is_operator(std::string str)
{
	int total_operators = sizeof(valid_operators) / MAX_OPERATORS_SIZE;
	for (int i = 0; i < total_operators; i++)
	{
		const char* op = valid_operators[i];
		if(str == op)
		{
			return true;
		}
	}

	return false;
}

bool Lexer::is_number(char c)
{
	return c >= 48 && c <= 57;
}

bool Lexer::is_number(std::string number)
{
	for (int i = 0; i < number.size(); i++)
	{
		char c = number.at(i);
		if (!is_number(c))
			return false;
	}

	return true;
}

bool Lexer::is_string_seperator(char c)
{
	return c == '"';
}


bool Lexer::is_symbol(char c)
{
	int total_symbols = sizeof(symbols);
	for (int i = 0; i < total_symbols; i++)
	{
		if (c == symbols[i])
		{
			return true;
		}
	}

	return false;
}

int Lexer::is_character(char c)
{
	return !is_operator(c) && !is_symbol(c) && !is_number(c) && !is_whitespace(c);
}

bool Lexer::is_stackable(int token_type)
{
	return token_type == TOKEN_TYPE_OPERATOR || token_type == TOKEN_TYPE_NUMBER || token_type == TOKEN_TYPE_STRING;
}

std::string Lexer::get_operator(const char** ptr)
{
	const char* our_ptr = *ptr;
	char c = *our_ptr;
	std::string value = "";	
	while(is_operator(c))
	{
		value += c;
		our_ptr++;
		c = *our_ptr; 
	}

	if (value != "")
	{
		*ptr += value.length();
	}
	return value;
}

std::string Lexer::get_number(const char** ptr)
{
	const char* our_ptr = *ptr;
	char c = *our_ptr;
	std::string value = "";
	while(is_number(c))
	{
		value += c;
		our_ptr++;
		c = *our_ptr;
	}

	if (value != "")
	{
		*ptr += value.length();
	}
	return value;
}

std::string Lexer::get_string(const char** ptr)
{
	if (!is_string_seperator(**ptr))
	{
		throw std::logic_error("Expecting a string seperator for a string");
	}

	// Ok this appears to be a valid string so far, lets move the pointer forward to ignore the string seperator
	*ptr+=1;
	// our_ptr will now point at the start of the string
 	const char* our_ptr = *ptr;
	char c = *our_ptr;
	std::string value = "";	

	// Lets loop until we find an ending string seperator.
	while(!is_string_seperator(c))
	{
		value += c;
		our_ptr++;
		c = *our_ptr;
	}


	// Lets readjust the real pointer for the caller, we +1 due to the ending string seperator.
	*ptr += value.length() + 1;
	return value;
}


int Lexer::get_token_type_for_value(std::string token_value)
{
	if (is_keyword(token_value))
	{
		return TOKEN_TYPE_KEYWORD;
	}
	else if(is_operator(token_value))
	{
		return TOKEN_TYPE_OPERATOR;
	}
	else if(is_number(token_value))
	{
		return TOKEN_TYPE_NUMBER;
	}
	else if(is_string_seperator(token_value.at(0)))
	{
		return TOKEN_TYPE_STRING;
	}
	else if(is_symbol(token_value.at(0)))
	{
		return TOKEN_TYPE_SYMBOL;
	}

	// We will assume this is an identifier
	return TOKEN_TYPE_IDENTIFIER;
}

std::string Lexer::handle_stackables(int token_type, std::string token_value, const char** ptr)
{
	const char* our_ptr = *ptr;
	switch (token_type)
	{
		case TOKEN_TYPE_OPERATOR:
		{
			token_value = get_operator(ptr);
		}
		break;

		case TOKEN_TYPE_NUMBER:
		{
			token_value = get_number(ptr);
		}
		break;

		case TOKEN_TYPE_STRING:
		{
			token_value = get_string(ptr);
		}
		break;
	}


	return token_value;
}
/**
 * Stage 1 will remove all comments, and create tokens based on the input
*/
Token* Lexer::stage1()
{
	Token* root_token = NULL;
	Token* last_token = NULL;
	const char* ptr = this->buf;
	// We will loop through the whole thing and when we reach a whitespace a token has been completed
	while (ptr < this->end)
	{
		std::string token_value = "";
		char c = *ptr;
		if (!is_whitespace(c))
		{
			int token_type = -1;
			if(is_character(c))
			{
				const char* c_ptr = ptr;
				while(is_character(c))
				{
					token_value += c;
					c_ptr+=1;
					c = *c_ptr;					
				}
				token_type = get_token_type_for_value(token_value);
				ptr += token_value.size();
			}
			else
			{
				token_value += c;
				token_type = get_token_type_for_value(token_value);
				if (is_stackable(token_type))
				{
					// Handle the stackables, ptr is automatically adjusted correctly
					token_value = handle_stackables(token_type, token_value, &ptr);
				}
				else
				{
					// This token type is non-stackable so lets just proceed further
					ptr += 1;
				}
			}

			Token* new_token = new Token(token_type);
			new_token->setValue(token_value);
			if (root_token == NULL)
			{
				root_token = new_token;
				last_token = new_token;
			}
			else
			{
				last_token->next = new_token;
				last_token = new_token;
			}
		}
		else
		{
			// Ignore all whitespace.
			ptr += 1;
		}
	}

	return root_token;
}

/**
 * Stage 2 will ensure that the tokens values are valid.
 * Such as a stacked value is correct, e.g a stacked operator value "++--" is clearly illegal.
*/
void Lexer::stage2(Token* root_token)
{
	Token* token = root_token;
	while(token != NULL)
	{
		int token_type = token->getType();
		struct value token_value = token->getValue();
		std::string token_svalue = token_value.svalue;
		switch(token_type)
		{
			case TOKEN_TYPE_OPERATOR:
			{
				if (!is_operator(token_svalue))
				{
					throw std::logic_error("Invalid operator");
				}
			}
			break;
		}

		token = token->next;
	}
}

Token* Lexer::lex()
{
	if (this->root != NULL)
	{
		throw std::logic_error("Token* Lexer::lex(): You must call \"cleanup()\" before calling \"lex()\" again");
	}
	// Stage 1 - Remove comments; Create tokens
	this->root = stage1();
	// Stage 2 - Identify errors such as illegal operators
	stage2(this->root);

	return this->root;
}
