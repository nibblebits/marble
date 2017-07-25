#include "lexer.h"
#include <vector>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <memory>
#include "statics.h"
#include "config.h"
const char keywords[][MAX_KEYWORD_SIZE] = {"public", "private", "protected"};
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

/**
 * Stage 1 will remove all comments, and create string tokens. 
 * For all other data they become an unknown token to be processed further in lexical analysis.
*/
void Lexer::stage1(std::vector<std::shared_ptr<Token>>* tokens)
{
	std::shared_ptr<Token> token = NULL;
	const char* ptr = this->buf;
	std::string token_value = "";	
	// We will loop through the whole thing and when we reach a whitespace a token has been completed
	for (int i = 0; i < this->size; i++)
	{
		char c = *ptr;
		if (is_whitespace(c))
		{
			// We don't want to make tokens for white spaces. So lets first check we have a token value
			if (token_value != "")
			{
				token = std::shared_ptr<Token>(new Token(TOKEN_TYPE_UNKNOWN));
				token->setValue(token_value);
				tokens->push_back(token);
				token_value = "";
			}
		}
		else
		{
			token_value += c;
		}
		ptr++;	
	}

	// We have reached the end now. Is the token value non empty? If so then we should class this as a token
	if (token_value != "")
	{
		token = std::shared_ptr<Token>(new Token(TOKEN_TYPE_UNKNOWN));
		token->setValue(token_value);
		tokens->push_back(token);
	}
}

void Lexer::stage2(std::vector<std::shared_ptr<Token>>* tokens)
{
	// Ok lets resolve unknown tokens
	for (int i = 0; i < tokens->size(); i++)
	{
		std::shared_ptr<Token> token = tokens->at(i);
		if (is_keyword(token->getValue().svalue))
		{
			token->setType(TOKEN_TYPE_KEYWORD);
		}
	}
}

const std::vector<std::shared_ptr<Token>> Lexer::lex()
{
	std::vector<std::shared_ptr<Token>> tokens;
	
	// Stage 1 - Remove comments; Create string tokens; Create unknown tokens for all words;
	stage1(&tokens);
	// Stage 2 - Resolve unknown tokens
	stage2(&tokens);

	return tokens;
}
