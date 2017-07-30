#include "token.h"
#include "statics.h"
#include <stdlib.h>
Token::Token(int type)
{
    this->type = type;
    this->next = NULL;
}

Token::~Token()
{

}

void Token::setType(int type)
{
	this->type = type;
}

int Token::getType()
{
	return this->type;
}

void Token::setValue(std::string svalue)
{
	this->value.dvalue = atof(svalue.c_str());
	this->value.svalue = svalue;
}

struct value Token::getValue()
{
	return this->value;
}


bool Token::isString()
{
	return getType() == TOKEN_TYPE_STRING;
}

bool Token::isString(std::string value)
{
	return getType() == TOKEN_TYPE_STRING && getValue().svalue == value;
}

bool Token::isSymbol()
{
	return getType() == TOKEN_TYPE_SYMBOL;
}

bool Token::isSymbol(std::string value)
{
	return getType() == TOKEN_TYPE_SYMBOL && getValue().svalue == value;
}

bool Token::isIdentifier()
{
	return getType() == TOKEN_TYPE_IDENTIFIER;
}

bool Token::isIdentifier(std::string value)
{
	return getType() == TOKEN_TYPE_IDENTIFIER && getValue().svalue == value;
}

bool Token::isKeyword()
{	
	return getType() == TOKEN_TYPE_KEYWORD;
}

bool Token::isKeyword(std::string value)
{
	return getType() == TOKEN_TYPE_KEYWORD && getValue().svalue == value;
}

bool Token::isOperator()
{
	return getType() == TOKEN_TYPE_OPERATOR;
}

bool Token::isOperator(std::string value)
{
	return getType() == TOKEN_TYPE_OPERATOR && getValue().svalue == value;
}

bool Token::isNumber()
{
	return getType() == TOKEN_TYPE_NUMBER;
}

bool Token::isNumber(std::string value)
{
	return getType() == TOKEN_TYPE_NUMBER && getValue().svalue == value;
}

bool Token::isLiteral()
{
	return this->type == TOKEN_TYPE_NUMBER || this->type == TOKEN_TYPE_IDENTIFIER;
}

