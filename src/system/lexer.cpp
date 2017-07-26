#include "lexer.h"
#include <vector>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <memory>
#include "statics.h"
#include "config.h"
const char keywords[][MAX_KEYWORD_SIZE] = {"public", "private", "protected"};
const char valid_operators[][MAX_OPERATORS_SIZE] = {"+", "-", "*", "/", "++", "--", "+=", "-=", "/=", "*=", "-=", "="};
const char symbols[] = {';',',','(', ')', '{', '}','[',']'};
Lexer::Lexer()
{

}
Lexer::~Lexer()
{

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
	return -1;
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
void Lexer::stage1(std::vector<std::shared_ptr<Token>>* tokens)
{
	std::shared_ptr<Token> token = NULL;
	const char* ptr = this->buf;
	std::string token_value = "";
	// We will loop through the whole thing and when we reach a whitespace a token has been completed
	while (ptr < this->end)
	{
		char c = *ptr;
		if (!is_whitespace(c))
		{
			token_value += c;
			int token_type = get_token_type_for_value(token_value);
			if (token_type != -1)
			{
				token = std::shared_ptr<Token>(new Token(token_type));
				// Lets attempt to stack the token value if its non-stackable the original token value will be returned
				token_value = handle_stackables(token_type, token_value, &ptr);			
			}
			
		}
		else if (token_value != "" && token == NULL)
		{
			// This must be an identifier as the token value is present and we have reached a whitespace but no token has been set yet
			token = std::shared_ptr<Token>(new Token(TOKEN_TYPE_IDENTIFIER));
		}
		

		// If the token is not NULL then we are ready to set its token value.
		if (token != NULL)
		{
			token->setValue(token_value);
			tokens->push_back(token);
			token_value = "";
			token = NULL;		
		}
		ptr++;	
	}

	// We have reached the end now. If we still have a token value at this point there is an error
	if (token_value != "")
	{
		throw std::logic_error("Invalid input");
	}
}

/**
 * Stage 2 will ensure that the tokens values are valid.
 * Such as a stacked value is correct, e.g a stacked operator value "++--" is clearly illegal.
*/
void Lexer::stage2(std::vector<std::shared_ptr<Token>>* tokens)
{
	for (int i = 0; i < tokens->size(); i++)
	{
		std::shared_ptr<Token> token = tokens->at(i);
		int token_type = token->getType();
		struct token_value token_value = token->getValue();
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
	}
}

const std::vector<std::shared_ptr<Token>> Lexer::lex()
{
	std::vector<std::shared_ptr<Token>> tokens;
	
	// Stage 1 - Remove comments; Create tokens
	stage1(&tokens);
	// Stage 2 - Identify errors such as illegal operators
	stage2(&tokens);

	return tokens;
}
