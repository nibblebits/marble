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
	return -1;
}

std::string Lexer::handle_stackables(int token_type, std::string token_value, const char** ptr)
{
	const char* our_ptr = *ptr;
	char c = *our_ptr;
	bool did_stack = false;
	switch (token_type)
	{
		case TOKEN_TYPE_OPERATOR:
		{
			token_value = "";
			while(is_operator(c))
			{
				token_value += c;
				our_ptr++;
				c = *our_ptr; 
			}

			did_stack = true;
		}
		break;

		case TOKEN_TYPE_NUMBER:
		{
			token_value = "";
			while(is_number(c))
			{
				token_value += c;
				our_ptr++;
				c = *our_ptr;
			}
			did_stack = true;
		}
		break;
	}

	// If we did stack a token value then lets readjust the pointer so that its valid for when we leave.
	if (did_stack)
	{
		// Ok lets readjust the real pointer now so that execution can continue properly after leaving this method
		*ptr += token_value.size();
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
